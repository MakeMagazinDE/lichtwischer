#!/usr/bin/env python

import sys
import os

class PNMdata( object ):

	def __init__( self, name ):
		tokens = []

		file = open( name, 'rb' )

		self.type = file.readline().split()[0]
		print self.type

		line = file.readline()
		while line[0] == '#':
			line = file.readline()
		width, height = line.split()
		self.width = int( width )
		self.height = int( height )

		if self.type == "P1":
			self.maxcolor = 1
			self.components = 1
			binary = False
		elif self.type == "P2":
			self.maxcolor = int( file.readline() )
			self.components = 1
			binary = False
		elif self.type == "P3":
			self.maxcolor = int( file.readline() )
			self.components = 3
			binary = False
		elif self.type == "P4":
			self.maxcolor = 1
			self.components = 1
			binary = True
		elif self.type == "P5":
			self.maxcolor = int( file.readline() )
			self.components = 1
			binary = True
		elif self.type == "P6":
			self.maxcolor = int( file.readline() )
			self.components = 3
			binary = True
		else:
			print >> sys.stderr, "FEHLER: Dateiformat nicht unterstuetzt in ", name
			sys.exit( 5 )

		values = []
		if binary:
			try:
			    byte = file.read( 1 )
			    while byte:
			        values.append( ord( byte ) )
			        byte = file.read( 1 )
			finally:
				file.close()
		else:
			for line in file:	
				for word in line.split():
					if word[0] == '#':
						break
					else:
						values.append( int( word ) )
			file.close()		

		print values

		self.data = []
		for y in range( self.height ):
			line = []
			for x in range( self.width ):
				color = []
				for component in range( self.components ):
					offset = y*self.width*self.components + x*self.components + component
					try:
						value = values[offset]
					except:
						value = 0
					color.append( value )
				line.append( color )
			self.data.append( line )

	def rect( self, xpos, ypos, width, height ):
		data = []
		for y in range( ypos, ypos+height ):
			line = []
			for x in range( xpos, xpos+width ):
				try:
					color = self.data[y][x]
				except:
					color = []
				if color:
					line.append( color )
			data.append( line )
		return data


class LWframe( object ):

	def __init__( self, data ):

		self.data = []
		for inline in data:
			line = []
			for incolor in inline:
				sum = 0
				for c in incolor:
					sum += c
				if sum == 0:
					line.append( 0 )
				else:
					line.append( 1 )
			self.data.append( line )
		self.height = len( self.data )
		self.width = len( self.data[0] )

	def to_include( self, invert=True, revert=False, ypadding=0 ):
		frame = []
		for x in range( self.width ):
			sum = 0
			if revert:
				rg = range( self.height )
			else:
				rg = range( self.height-1, -1, -1 )
			for y in rg:
				sum <<= 1
				inc = self.data[y][x]
				if invert:
					inc = 1-inc
				sum += inc
			sum <<= ypadding
			frame.append( str( sum ) )
		return ','.join( frame )


class LWdata( object ):

	def __init__( self, screenx, screeny, border=0, invert=True, revert=False, frame=False ):
		self.screenx = screenx
		self.screeny = screeny
		self.border = border 
		self.invert = invert
		self.revert = revert
		self.frames = []
		if frame:
			self.frames.append( frame )

	def __add__( self, plus ):
		self.frames.append( plus )
		return self

	def __str__( self ):
		txt = '#define ANIM_XSIZE ' + str( self.screenx+self.border*2 ) + '\n'
		txt += '#define ANIM_YSIZE ' + str( self.screeny ) + '\n'
		txt += '#define ANIM_FRAMES ' + str( len( self.frames ) ) + '\n'
		txt += '#define ANIM_LENGTH ' + str( len( self.frames ) ) + '\n'

		txt += '\n\n'

		txt += 'static int animSequence[] = {\n  '
		txt += ','.join( [ str( x ) for x in range( len( self.frames ) ) ] )
		txt += '\n};\n'

		txt += '\n\n'

		txt += 'PROGMEM prog_uint16_t pgmAnimData[] = {\n'

		for frame in self.frames:
			line = '  '
			line += '0,'*self.border

			leftpad = (self.screenx - frame.width)/2
			rightpad = (self.screenx-frame.width-leftpad)
			toppad = (self.screeny - frame.height)/2
			framestr = '0,'*leftpad
			framestr += frame.to_include( self.invert, self.revert, toppad )
			framestr += ','
			framestr += '0,'*rightpad
			line += framestr

			line += '0,'*self.border

			txt += line
			txt += '\n'

		txt += '};'

		return txt



###################################################################################################
# main

from optparse import OptionParser

usage = "usage: %prog [Optionen] <PNM-Datei>"
parser = OptionParser( usage=usage, version="%prog" )
parser.add_option( "-x", "--screenx", dest="screen_x", default=28,
                   help="Breite des Schwebebildschirmes (SCREEN_X in der Firmware, Default 28)" )
parser.add_option( "-y", "--screeny", dest="screen_y", default=10,
	               help="Laenge der LED-Leiste (SCREEN_Y in der Firmware, Default 10)" )
parser.add_option( "-b", "--border", dest="border", default=2,
	               help="Breite des horizontalen Randes (BORDER in der Firmware, Default 2)" )
parser.add_option( "-s", "--speed", dest="speed", default=3,
	               help="Scroll-Geschwindigkeit fuer ueberbreite Bilder (Default 3)" )
parser.add_option( "-n", "--noinvert", action="store_false", dest="invert", default=True,
                   help="Pixel nicht invertieren" )
parser.add_option( "-r", "--revert", action="store_true", dest="revert", default=False,
                   help="Y-Achse umdrehen" )
(opt, args) = parser.parse_args()

print opt, args

if len( args ) == 0:
    print >> sys.stderr, "FEHLER: PNM-Dateiname fehlt"
    sys.exit( 5 )

frames = LWdata( opt.screen_x, opt.screen_y, opt.border, opt.invert, opt.revert )
for file in args:
	if not os.path.isfile( file ):
		print >> sys.stderr, "WARNUNG: Fehler beim Oeffnen von", file
		continue
	pnm = PNMdata( file )
	startx = 0
	endx = pnm.width-opt.screen_x
	steps = range( startx, endx, opt.speed )
	if not steps:
		steps = [0]
	print startx, endx, steps
	for offset in steps:
		rect = pnm.rect( offset, 0, opt.screen_x, opt.screen_y )
		frames += LWframe( rect )

print frames

