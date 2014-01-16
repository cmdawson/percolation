var LatticeView = new function() {
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

	//var camera = new BABYLON.FreeCamera("Camera", 
	//    new BABYLON.Vector3(0, 0, -15), this.scene);
	//camera.cameraDirection = new BABYLON.Vector3(0,-15,-15);
	var postProcess = new BABYLON.FxaaPostProcess("fxaa", 1.0, this.camera);

	var light0 = new BABYLON.PointLight("Omni", new BABYLON.Vector3(0, 30, -30),
	    this.scene);

	this.scene.fogMode = BABYLON.Scene.FOGMODE_EXP;
	this.scene.fogDensity = 0.05;

	this.scene.activeCamera.attachControl(this.canvas);
    }

    this.load = function() {

	var material = new BABYLON.StandardMaterial("material01", this.scene);
	material.diffuseColor = new BABYLON.Color3(1, 0, 0);

	var me = this;

	$.getJSON( "pyrochlore3.json", function(data,error) {

		var camtgt = new BABYLON.Vector3(0,0,0);

		data.vert.forEach(function(v) {
		    var vertex = new BABYLON.Mesh.CreateSphere("v", 12, me.RADIUS, me.scene);
		    vertex.material = material;
		    vertex.position = new BABYLON.Vector3(v.x, v.y, v.z);
		    me.vertices.push(vertex);

		    camtgt.addInPlace(vertex.position);
		});


		camtgt.scaleInPlace(1.0/me.vertices.length);
		me.camera.target = camtgt;

		//me.camera.position.z = -50*camtgt.z;
		//console.warn(camtgt.z);

		data.edge.forEach(function(e) {
		    var p0 = me.vertices[e[0]].position;
		    var p1 = me.vertices[e[1]].position;
		    var diff = p1.subtract(p0);
		    var cyln = diff.length();

		    var edge = new BABYLON.Mesh.CreateCylinder("cylinder", cyln, 
			me.RADIUS/4.0, me.RADIUS/4.0, 6, me.scene, false);
		    edge.parent = me.vertices[e[0]];

		    // angle we need to rotate by
		    var alpha = Math.acos(diff.y/diff.length());
		    
		    // vector we need to rotate around is (diff.z, 0, -diff.x)
		    var axis_norm = Math.sqrt(diff.z*diff.z+diff.x*diff.x);
    
		    edge.rotationQuaternion = new BABYLON.Quaternion(
			Math.sin(alpha/2)*diff.z/axis_norm, 0.0,
			-Math.sin(alpha/2)*diff.x/axis_norm, Math.cos(alpha/2) );

		    edge.position.x = diff.x/2.0;
		    edge.position.y = diff.y/2.0;
		    edge.position.z = diff.z/2.0;

		    me.edges.push(edge);
		});
	});
    }

    this.render = function() {
	var scene = this.scene;
	//this.scene.activeCamera.attachControl(this.canvas);
	this.engine.runRenderLoop(function () {
	    scene.render();
	});
    }
}
