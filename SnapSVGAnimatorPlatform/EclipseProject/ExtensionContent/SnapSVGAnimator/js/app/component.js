
/**
 * SVGAnim
 * initialize animation component
 *
 */
function SVGAnim(data, w, h) {
    var instance = this,
        timeline,
        cbk;

    w = w || 100;
    h = h || 100;

    SVGAnim.prototype.toString = function () {
        console.log('hi');
        return "SnapSVGAnimator v" + this.version;
    };

    instance.resourceManager = new ResourceManager(data);

    //TODO:: RENDERER
    instance.s = new Snap(w, h);
    id = instance.s.id;
    instance.s.attr('id', id);

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
            cbk = setTimeout(loop, 1000 / fps);
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

    function loop() {

        instance.movieclip.play(instance.resourceManager);

        if (instance.playing) {
            clearTimeout(cbk);
            cbk = setTimeout(loop, 1000 / fps);
        }

        if (instance.debug) {
            traceDisplayList();
        }
    }

    function handleKeyDown(e) {
        if (e.keyCode == 39) { //right key press
            loop();
        }
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
        traceChildren(2, instance.movieclip);

        debug.innerHTML = str;
    }

    function reset() {

    }

}

