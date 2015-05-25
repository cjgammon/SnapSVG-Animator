/*global define $ requestAnimationFrame*/

define(function (require) {
	
	var Component,
		ResourceManager = require('app/resourceManager'),
		MovieClip = require('app/stage');
	
	require('snap');

    Component = function (data, w, h) {
		var instance = this,
			timeline,
			cbk = undefined;

		instance.resourceManager = new ResourceManager(data);
		instance.s = new Snap(w, h);
		instance.s.attr('id', instance.s.id);
		create(instance.s);
    
		//timeline = instance.movieclip.getTimeline();
		/*
		this.play = function () {
			instance.movieclip.play(instance.resourceManager);
		}
		
		this.timeline = function () {
			return timeline;
		}
		*/

		function create(s) {
			var maintimelineIndex,
				mainTimeline;
			
			if(instance.rootAnimator !== undefined) 
			{
				instance.rootAnimator.dispose();
				delete rootAnimator;
			}
			
			maintimelineIndex = instance.resourceManager.m_data.DOMDocument.Timeline.length - 1;
			mainTimeline = instance.resourceManager.m_data.DOMDocument.Timeline[maintimelineIndex];
			instance.movieclip = new MovieClip(instance.s, instance.s, mainTimeline, instance.resourceManager);
		};


		this.play = function () {
			if (cbk === undefined) {
				cbk = setTimeout(loop, 1000 / fps);
			}
		}

		this.pause = function () {
			if(cbk !== undefined) 
			{
				clearTimeout(cbk);
				cbk = undefined;
			}	
		}

		this.stop = function () {
			this.pause();
			reset();
		}

		function loop() {
			instance.movieclip.play(instance.resourceManager);
			cbk = setTimeout(loop, 1000 / fps)
		}

		function reset() {

		}
	};

	return Component;
});