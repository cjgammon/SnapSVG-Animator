/*global define $ requestAnimationFrame*/

define(function (require) {
	
	var Component,
		ResourceManager = require('app/resourceManager'),
		MovieClip = require('app/stage');
	
	require('snap');

    Component = function (data, w, h) {
		var instance = this,
			timeline;

		instance.resourceManager = new ResourceManager(data);
		
		instance.s = new Snap(w, h);
		create(instance.s);
    
		timeline = instance.movieclip.getTimeline();
		
		this.play = function () {
			timeline.play();
		}
		
		this.timeline = function () {
			return timeline;
		}

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
			
			instance.movieclip = new MovieClip(instance.s, mainTimeline, instance.resourceManager);
		}
	};

	return Component;
});