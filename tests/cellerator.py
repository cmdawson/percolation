#!/usr/bin/python3
from numpy import sin,cos,pi,array
import json

vertices = []
edges = []

xa = []

for ii in range(6):
	x = [sin(ii*pi/3.0), cos(ii*pi/3.0)]
	xa.append(array(x))
	vertices.append({'x':x, 'id':ii})
	edges.append({'src':ii, 'tgt':(ii+1)%6})

pdata = {}
pdata['vertices'] = vertices
pdata['edges'] = edges
pdata['axes'] = [list(xa[0]-xa[4]), list(xa[2]-xa[4])]

print(json.dumps(pdata))

"""
{
"axes":
	[
		[1.0,0.1,0.2],
		[0.2, 0.33, 1]
	],
"vertices":
	[
		{"x":[0,0,0], "id":0, "c":0},
		{"x":[1,2,0], "id":1, "c":"0"}
		...
	]
"edges":
	[
		{"src":0, "tgt":1, "c":2},
		{"src":1, "tgt":2, "c":2},
		...
	]
}
"""

