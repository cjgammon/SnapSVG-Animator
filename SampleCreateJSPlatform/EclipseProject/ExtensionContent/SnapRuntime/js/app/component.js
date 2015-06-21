/*global define $ requestAnimationFrame*/

define(function (require) {
	
	var Component,
		ResourceManager = require('app/resourceManager'),
		MovieClip = require('app/movieclip');
	
	require('snap');

    Component = function (id, data, w, h) {
		var instance = this,
			timeline,
			cbk;

		instance.resourceManager = new ResourceManager(data);
		instance.s = new Snap(w, h);
		instance.s.attr('id', id);
        instance.playing = false;
		create(instance.s);

        window.addEventListener('keydown', function (e) {
            if (e.keyCode == 39) {
                loop();
            }
        });

		function create(s) {
			var maintimelineIndex,
				mainTimeline;
			
			if(instance.rootAnimator !== undefined) 
			{
				instance.rootAnimator.dispose();
			}
			
			maintimelineIndex = instance.resourceManager.m_data.DOMDocument.Timeline.length - 1;
			mainTimeline = instance.resourceManager.m_data.DOMDocument.Timeline[maintimelineIndex];
			instance.movieclip = new MovieClip(instance.s, mainTimeline, instance.resourceManager, id);
		}

		this.play = function () {
			if (cbk === undefined) {
				cbk = setTimeout(loop, 1000 / fps);
			}
		};

		this.pause = function () {
			if(cbk !== undefined) 
			{
				clearTimeout(cbk);
				cbk = undefined;
			}	
		};

		this.stop = function () {
			this.pause();
			reset();
		};

		function loop() {

			instance.movieclip.play(instance.resourceManager);

            if (instance.playing) {
                clearTimeout(cbk);
                cbk = setTimeout(loop, 1000 / fps);
            }

            traceDisplayList();
		}

        function traceDisplayList() {
            var debug = document.getElementById('debug'),
                str = '';

            if (!debug) { 
                debug = document.createElement('div');
                debug.id = 'debug';
                debug.style.position = 'absolute';
                debug.style.top = '0';
                debug.style.right = '0';
                debug.style.backgroundColor = 'black';
                debug.style.color = 'white';
                debug.style.padding = '1em';

                document.body.appendChild(debug);
            }

            function traceChildren(j, el) {
                var k, i;

                for (i = 0; i < el.children.length; i += 1) {

                    for (k = 0; k < j; k += 1) {
                        str += '-';
                    }
                    str += el.children[i].id + ':' + el.children[i].children.length + '<br/>';

                    traceChildren(j + 5, el.children[i]);
                }
            }
         
            str += instance.movieclip.id + '<br/>';
            traceChildren(2, instance.movieclip);

            debug.innerHTML = str;
        }

		function reset() {

		}
	};

	return Component;
});
