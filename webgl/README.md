Glattice - Rudimentary WebGL graph and lattice viewer
--------------------------------------------------------

Glattice is very basic viewer for 3d graphs and lattices built using the excellent Babylon framework for WebGL. It reads in a JSON description of a graph's vertices and edges, then renders them as a bunch of spheres and cylinders. There are about a gazillion other things out there that will also do this, but the nice thing about using a framework like Babylon is that it allows full control over the rendering. So if you're fussy about your 3D then you can tweak away. In my case I needed to view some reasonably large pyrochlore lattices which looked much better after adding some fog and turning on anti-aliasing. 


Usage involves including the following bits in your HTML
```html
<!-- Some header stuff -->

<script src="babylon.1.8.0.js"></script>
<script src="Glattice.js"></script>

<!-- some body content -->

<canvas id="renderCanvas"></canvas>

<!-- Starting up Glattice and loading/rendering a file -->

<script type="text/javascript">
    Glattice.init(document.getElementById("renderCanvas"));
    Glattice.load("alattice.json");
</script>
```
Note that if you load such a page from your local filesystem you'll need to ensure that the browser allows local javascript access to files, e.g. on chromium use the switch --allow-file-access-from-files.

The JSON consists of two lists, one for vertex coordinates (x,y,z), and one for edges specified as a pair of vertice indices (i,j). For example a 5x5 square looks this
```json
{
    "vert":[
	{"x":0,"y":0,"z":0},
	{"x":5,"y":0,"z":0},
	{"x":5,"y":5,"z":0},
	{"x":0,"y":5,"z":0},
    ]
    "edge":[
	[0,1],
	[1,2],
	[2,3],
	[3,0]
    ]
}
```
The example.html file also demonstrates adding a simple control for selecting different lattice files. 

Comments welcome as always!
