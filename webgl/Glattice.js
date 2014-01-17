var Glattice = new function() {
    this.canvas = null;
    this.engine = null;
    this.scene = null;
    this.camera = null;

    this.vertices = [];
    this.edges = [];

    this.RADIUS = 0.5;

    this.init = function(canvas) {
	this.canvas = canvas;
	// Load BABYLON 3D engine
	this.engine = new BABYLON.Engine(this.canvas, true);
	this.scene = new BABYLON.Scene(this.engine);
	
	// Camera looking to the zero point (0,0,0)
	this.camera = new BABYLON.ArcRotateCamera("Camera", 1, 0.8, 10,
	    new BABYLON.Vector3(0, 0, 0), this.scene);
	this.camera.setPosition(new BABYLON.Vector3(0, 0, -15));

	this.camera.angularSensibility = 500;

	//var camera = new BABYLON.FreeCamera("Camera", 
	//    new BABYLON.Vector3(0, 0, -15), this.scene);
	//camera.cameraDirection = new BABYLON.Vector3(0,-15,-15);
	var postProcess = new BABYLON.FxaaPostProcess("fxaa", 1.0, this.camera);

	var light0 = new BABYLON.PointLight("Omni", new BABYLON.Vector3(0, 30, -20),
	    this.scene);
	var light1 = new BABYLON.DirectionalLight("Direct",
	    new BABYLON.Vector3(0, 30, 0), this.scene);
	light1.intensity = 0.2;

	this.scene.fogMode = BABYLON.Scene.FOGMODE_EXP;
	this.scene.fogDensity = 0.04;

	var material = new BABYLON.StandardMaterial("material01", this.scene);
	material.diffuseColor = new BABYLON.Color3(1, 0, 0);

	// Template vertex and edge we will clone as necessary
	this.vertices[0] = new BABYLON.Mesh.CreateSphere("base_vertex", 12, 
	    this.RADIUS, this.scene);
	this.vertices[0].material = material;

	this.edges[0] = new BABYLON.Mesh.CreateCylinder("cylinder", 1.0, 
	    this.RADIUS/4.0, this.RADIUS/4, 6, this.scene, false);

	this.scene.activeCamera.attachControl(this.canvas);
    }

    this.load = function(jsonfile) {
	//var material = new BABYLON.StandardMaterial("material01", this.scene);
	//material.diffuseColor = new BABYLON.Color3(1, 0, 0);

	this.engine.stopRenderLoop(); 
	this.scene.activeCamera.detachControl(this.canvas);

	var nverts = this.vertices.length;
	var nedges = this.edges.length;

	var camtgt = new BABYLON.Vector3(0,0,0);
	
	var me = this;
	$.getJSON(jsonfile, function(data,error) {

	    // Remove vertices and edges if necessary
	    if (data.edge.length < nedges)
	    {
		me.edges.splice(data.edge.length, 1+nedges-data.edge.length).forEach(function(e) {e.dispose();});
		nedges = me.edges.length;
	    }
	    if (data.vert.length < nverts)
	    {
		me.vertices.splice(data.vert.length, 1+nverts-data.vert.length).forEach(function(v) {v.dispose(true);})
		nverts = me.vertices.length;
	    }

	    var ii = 0;
	    for (ii=0;ii<nverts;ii++) {
		var vpos = data.vert[ii];
		me.vertices[ii].position.x = vpos.x;
		me.vertices[ii].position.y = vpos.y;
		me.vertices[ii].position.z = vpos.z;
		camtgt.x += vpos.x; camtgt.y += vpos.y; camtgt.z += vpos.z;
	    }
	    for (;ii<data.vert.length;ii++)
	    {
		var vpos = data.vert[ii];
		var newv = me.vertices[0].clone("v"+ii,null,true);
		newv.position.x = vpos.x;
		newv.position.y = vpos.y;
		newv.position.z = vpos.z;
		me.vertices.push(newv);
		camtgt.x += vpos.x; camtgt.y += vpos.y; camtgt.z += vpos.z;
	    }

	    // target ArcRotateCamera at lattice centroid
	    camtgt.scaleInPlace(1.0/me.vertices.length);
	    me.camera.target = camtgt;

	    for (ii=0;ii<nedges;ii++) {
		var ee = data.edge[ii];
		var p0 = me.vertices[ee[1]].position;
		var p1 = me.vertices[ee[0]].position;
		var diff = p1.subtract(p0);
		var scale = diff.length();

		var _edge = me.edges[ii];
		_edge.parent = me.vertices[ee[1]];

		// angle we need to rotate by
		var alpha = Math.acos(diff.y/diff.length());
		
		// vector we need to rotate around is (diff.z, 0, -diff.x)
		var axis_norm = Math.sqrt(diff.z*diff.z+diff.x*diff.x);
		_edge.position.x = diff.x/2.0;
		_edge.position.y = diff.y/2.0;
		_edge.position.z = diff.z/2.0;
		_edge.scaling.y = scale;

		_edge.rotationQuaternion = new BABYLON.Quaternion(
		    Math.sin(alpha/2)*diff.z/axis_norm, 0.0,
		    -Math.sin(alpha/2)*diff.x/axis_norm, Math.cos(alpha/2) );
	    }
	    for (;ii<data.edge.length;ii++)
	    {
		var ee = data.edge[ii];
		var _newedge = me.edges[0].clone("e"+ii);

		var p0 = me.vertices[ee[0]].position;
		var p1 = me.vertices[ee[1]].position;
		var diff = p1.subtract(p0);
		var scale = diff.length();
		_newedge.parent = me.vertices[ee[0]];

		// angle we need to rotate by
		var alpha = Math.acos(diff.y/diff.length());
		
		// vector we need to rotate around is (diff.z, 0, -diff.x)
		var axis_norm = Math.sqrt(diff.z*diff.z+diff.x*diff.x);

		_newedge.position.x = diff.x/2.0;
		_newedge.position.y = diff.y/2.0;
		_newedge.position.z = diff.z/2.0;
		_newedge.scaling.y = scale;

		_newedge.rotationQuaternion = new BABYLON.Quaternion(
		    Math.sin(alpha/2)*diff.z/axis_norm, 0.0,
		    -Math.sin(alpha/2)*diff.x/axis_norm, Math.cos(alpha/2) );

		me.edges.push(_newedge);
	    }
	});

	//document.getElementById("vtotal").innerHTML = "Total vertices:" + this.scene.getTotalVertices();
	this.scene.activeCamera.attachControl(this.canvas);
	this.render();
    }

    this.render = function() {
	var scene = this.scene;
	this.engine.runRenderLoop(function () {
	    scene.render();
	});
    }
}
