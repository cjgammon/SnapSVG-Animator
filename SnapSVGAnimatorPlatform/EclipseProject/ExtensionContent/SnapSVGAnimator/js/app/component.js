
/**
 * SVGAnim
 * initialize animation component
 *
 */
function SVGAnim(data, w, h, fps, params) {
    var instance = this,
        timeline,
        playing,
        autoplay,
        cbk,
        msg,
        color = '#008460',
        elementId;

    instance.version = '1.2.3';

    msg = 'Snap.svg Animator v' + instance.version;
    console.log("%c" + msg, "color:" + color + ";font-weight:bold;");

    params = params || {};
    fps = fps || 24;
    w = w || 100;
    h = h || 100;
    elementId = params.elementId;

    if (typeof(params.autoplay) !== 'undefined') {
        autoplay = params.autoplay;
    } else {
        autoplay = true;
    }
    playing = autoplay;

    instance.debug = false;

    if (instance.debug) {
        console.log("%c" + "/*DEBUG*/", "color:#FF6666;font-weight:bold;");
        playing = false;
        autoplay = false;
        window.addEventListener('keydown', handleKeyDown);
    }

    SVGAnim.prototype.toString = function () {
        return msg;
    };

    instance.MovieClip = MovieClip;

    instance.resourceManager = new ResourceManager(data);

    //TODO:: RENDERER
    instance.s = elementId ? new Snap(elementId) : new Snap(w, h);
    id = instance.s.id;
    instance.s.attr('id', id);
    instance.s.attr('viewBox', "0 0 " + w + " " + h);
    instance.s.attr('preserveAspectRatio', 'xMidYMid meet');  //TODO::make this adjustable
    //TODO:: set bg color here

    //Set width and height of element if element id given
    if(elementId) {
        instance.s.attr("width", w);
        instance.s.attr("height", h);
    }

    create(instance.s);

    function create(s) {
        var maintimelineIndex,
            mainTimeline,
            i;

        if(instance.rootAnimator !== undefined)
        {
            instance.rootAnimator.dispose();
        }

        //generate linked movieclips
        instance.linkage = {};

        for (i = data.DOMDocument.Timeline.length - 1; i > -1; i -= 1) {
          if (typeof(data.DOMDocument.Timeline[i].linkageName) !== 'undefined') {
            instance.linkage[data.DOMDocument.Timeline[i].linkageName] = data.DOMDocument.Timeline[i];
          } else {
            maintimelineIndex = i;
            break;
          }
        }

        mainTimeline = instance.resourceManager.m_data.DOMDocument.Timeline[maintimelineIndex];
        instance.mc = new MovieClip(mainTimeline, instance.s, instance.resourceManager, id);

        cbk = setTimeout(interval, 1000 / fps);
    }

    this.play = function () {
      instance.mc.play();
      playing = true;
    };

    this.stop = function () {

      if (instance.mc.m_currentFrameNo == 0) { //force render if called immediately
        instance.mc._animate();
      }

      instance.mc.stop();
      playing = false;
    };

    this.setLoop = function (l) {
        instance.mc.loops = l;
    };

    function interval() {
        if (playing) {
            instance.mc._animate();
        }
        clearTimeout(cbk);
        cbk = setTimeout(interval, 1000 / fps);
    }

    function handleKeyDown(e) {

        switch (e.keyCode) {
          case 65: //A inc frame by 1
           interval();
          break;
          case 32: //SPACE toggles playing
            if (instance.mc.playing) {
              instance.stop();
            } else {
              instance.play();
            }
          break;
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

        str += instance.mc.id + '<br/>';
        str += instance.mc.m_currentFrameNo + '<br/>';
        traceChildren(2, instance.mc);

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
