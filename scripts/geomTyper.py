#!/bin/env python

# geomTyper helps you by print out stuff you can cut and paste into your code
# for geometries

# Usage: python geomTyper.py /path/to/geom.fcl

import sys

class GeomEntry:
  def __init__(self, line):
    self.typ, self.name, self.value, self.units = self.parseLine(line)

  def parseLine(self, line):
    line = line.strip()  # Make sure no whitespace
    parts = line.split(":")  # Split at the colon
    parts = [aPart.strip() for aPart in parts]  # Strip the parts
    name = parts[0] # The name is to the left of the colon
    right = parts[1] # To the right of the colon

    # Set the units default
    units = 'mm'
    typ = 'double'

    # Find any comment?
    rparts = right.split('//')
    rparts = [aRpart.strip() for aRpart in rparts]
    value = rparts[0]  # Extract the value
    if len(rparts) > 1:
      # We have a comment!
      units = rparts[1]

    # Do we have a boolean value?
    if value.lower() in ("true", "false"):
      typ = "bool"
      units = ''
        
    # Don't have units for vis stuff
    if 'vis' in name.lower():
      units = ''

    # Do we have a vector?
    if value[0] == '[':
      typ = 'std::vector<double>' # Assume double
    if 'color' in name.lower():
        units = ''

    return typ, name, value, units

  def headerLine(self):
    const = 'const '
    if 'vector' in self.typ:
      const = ''
    print "%(const)s%(typ)s %(name)s;" % {'const': const, 'typ': self.typ, 'name': self.name}

  def paramLine(self):
    # if a vector, don't add units
    mult = '* ' + self.units
    if "vector" in self.typ or self.typ == 'bool' or self.units == '':
      mult = ''
    print '%(name)s( p.get<%(typ)s>("%(name)s") %(mult)s),' % \
      {'name': self.name, 'typ': self.typ, 'mult': mult}

  def printOutLine(self):
    if not 'vector' in self.typ:
      print 'oss << "  %(name)s=" << %(name)s << "\\n";' % {'name': self.name}
    else:
      print 'oss << "  %(name)s= "; for (auto entry : %(name)s) { oss << " " << entry; }; oss << "\\n";' % {'name': self.name}

  def applyUnitsToVectors(self):
    if 'vector' in self.typ and not 'bool' in self.typ and not 'color' in self.name.lower() and not 'vis' in self.name.lower() :
      print 'for (auto& entry : %(name)s ) { entry *= %(units)s; }' % \
        {'name': self.name, 'units': self.units}

def readFile(fileName):
  
  geomEntries = []
  for line in open(fileName).readlines():

    # Make sure we have a real line
    sline = line.strip()
    
    if len(sline) < 2:
      continue
    
    if sline[:2] == '//':
      continue

    if 'PROLOG' in sline:
      continue

    if '{' in sline or '}' in sline:
      continue

    # This should be a good line
    geomEntries.append( GeomEntry(sline) )

  return geomEntries

if __name__ == '__main__':
  filename = sys.argv[1]

  geomEntries = readFile(filename)

  print '-- CUT AND PASTE BELOW into class member data in header file --'
  [e.headerLine() for e in geomEntries]

  print '-- CUT AND PASTE BELOW into constructor initialization list --'
  [e.paramLine() for e in geomEntries]

  print '-- CUT AND PASTE BELOW into constructor body --'
  [e.applyUnitsToVectors() for e in geomEntries]

  print '-- CUT AND PASTE BELOW into the print method --'
  print '-- be sure to #include <sstream> --'
  # This one is a little tricker, because we have to deal with the vectors which
  # cannot be printed directly. So, make an ostringstream
  print 'std::ostringstream oss;'
  [e.printOutLine() for e in geomEntries]
  print 'mf::LogInfo("CATEGORY") << oss.str();'
