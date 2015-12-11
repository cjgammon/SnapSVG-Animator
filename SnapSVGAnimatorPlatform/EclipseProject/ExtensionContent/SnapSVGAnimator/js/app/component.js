
/**
 * SVGAnim
 * initialize animation component
 *
 */
function SVGAnim(data, w, h, fps, params) {
    var instance = this,
        timeline,
        autoplay,
        cbk;

    params = params|| {};
    fps = fps || 24;
    w = w || 100;
    h = h || 100;

    autoplay = params.autoplay;

    instance.debug = true;

    SVGAnim.prototype.toString = function () {
        return "SnapSVGAnimator v" + this.version;
    };

    instance.resourceManager = new ResourceManager(data);

    //TODO:: RENDERER
    instance.s = new Snap(w, h);
    id = instance.s.id;
    instance.s.attr('id', id);
    instance.s.attr('viewBox', "0 0 " + w + " " + h);
    instance.s.attr('preserveAspectRatio', 'xMidYMid meet');  //TODO::make this adjustable
    //TODO:: set bg color here

    create(instance.s);

    if (instance.debug) {
        window.addEventListener('keydown', handleKeyDown);
    }

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
        instance.playing = true;

        if (cbk === undefined) {
            cbk = setTimeout(interval, 1000 / fps);
        }
    };

    this.pause = function () {
        instance.playing = false;

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

    this.setLoop = function (l) {
        instance.movieclip.loops = l;
    };

    function interval() {
        console.log('interval');

        instance.movieclip._animate();

        if (instance.playing) {
            clearTimeout(cbk);
            cbk = setTimeout(interval, 1000 / fps);
        }
    }

    function handleKeyDown(e) {
        if (e.keyCode == 39) { //right key press
            interval();
        }
    }

    if (instance.debug) {
      setInterval(traceDisplayList, 100)
    }

    /**
     * debug output for displaylist
     *
     */
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
                str += el.children[i].id + ':' + el.children[i].children.length;

                if (el.children[i].isMask) {
                    str += ' (MASK till:' + el.children[i].maskTill + ')';
                }

                if (el.children[i].isMasked) {
                    str += ' (masked by: ' + el.children[i].mask + ')';
                }

                str += '<br/>';
                traceChildren(j + 5, el.children[i]);
            }
        }

        str += instance.movieclip.id + '<br/>';
        str += instance.movieclip.m_currentFrameNo + '<br/>';
        traceChildren(2, instance.movieclip);

        debug.innerHTML = str;
    }

    function reset() {

    }

    if (autoplay) {
        instance.play();
    } else {
        interval();
    }

}
