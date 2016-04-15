var SVGAnim = (function (root) {
    SVGAnim.version = "0.0.2";
   

var GarbagePool = function () {
    this.EMPTY_POOL = [];      //if empty remove
    this.REF_POOL = [];        //if no reference remove
};

GarbagePool.prototype.addEmpty = function (el) {
    this.EMPTY_POOL.push(el);
};

/**
 * manages adding of references
 *
 */
GarbagePool.prototype.addRef = function (el, refs) {
    var i,
        j;

    for (i = 0; i < this.REF_POOL.length; i += 1) {
        if (this.REF_POOL[i].el.id == el.id) {
            for (j = 0; j < refs.length; j += 1) {
                this.REF_POOL[i].refs.push(refs[j]);   
            }
            return;
        }
    }

    this.REF_POOL.push({el: el, refs: refs});
};

GarbagePool.prototype.purge = function () {
    this.purgeEmptyPool();
    this.purgeRefPool();
};

/**
 * check if empty and remove
 *
 */
GarbagePool.prototype.purgeEmptyPool = function () {
    var i,
        el;

    for (i = this.EMPTY_POOL.length - 1; i > -1; i -= 1) {
        el = this.EMPTY_POOL[i];
        if (el.children().length === 0) {
            el.remove();
            this.EMPTY_POOL.splice(i, 1);
        }
    }
};

/**
 * check if all references are removed then remove
 *
 */
GarbagePool.prototype.purgeRefPool = function () {
    var i,
        j,
        k,
        item;

    for (i = this.REF_POOL.length - 1; i > -1; i -= 1) {

        item = this.REF_POOL[i];
        k = 0;

        for (j = 0; j < item.refs.length; j += 1) {

            if (item.refs[j].removed) {
                k += 1;
            }

            if (k == item.refs.length) {
                item.el.remove();
                this.REF_POOL.splice(i, 1);
            }
        }
    }
};

var GP = new GarbagePool();

	
var Bitmap = function (parentMC,resourceManager,charId,ObjectId,placeAfter,transform) {
    var instance = this,
        parentEl = parentMC.el;

    this.create = function () {
        var j,
            k,
            transformArray,
            transformMat;

        instance.el = parentMC.el.g();
        instance.id = ObjectId;
        instance.el.attr({'class': 'shape', 'token': instance.id});

        instance.children = [];
        instance.isMask = false;
        instance.isMasked = false;
        instance.mask = null;
        instance.maskTill = null;

        for(var b =0;b<resourceManager.m_data.DOMDocument.Bitmaps.length;b++)
        {
            if(resourceManager.m_data.DOMDocument.Bitmaps[b].charid == charId)
            {
                var path = resourceManager.m_data.DOMDocument.Bitmaps[b].bitmapPath;
                var bitmap = parentMC.el.paper.image(path);
                instance.el.add(bitmap);
            }
        }

        transformArray = transform.split(",");
        transformMat = new Snap.Matrix(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5]);
        instance.el.transform(transformMat);

        if (placeAfter && parseInt(placeAfter) !== 0) {
            afterMC = parentMC.getChildById(parseInt(placeAfter));

            if (afterMC.isMasked) {  //if masked add outside mask group
                afterMC.el.parent().before(instance.el);
            } else {
                afterMC.el.before(instance.el);
            }

        } else {
            parentEl.add(instance.el); //TODO:: handle after
        }
    };

    this.create();
};

var Text = function (parentMC,resourceManager,charId,ObjectId,placeAfter,transform,bounds) {

    var instance = this,
        parentEl = parentMC.el;

    this.create = function () {
        var j,
            k,
            transformArray,
            transformMat;

        instance.el = parentMC.el.g();
        instance.id = ObjectId;
        instance.el.attr({'class': 'text', 'token': instance.id});

        instance.children = [];
        instance.isMask = false;
        instance.isMasked = false;
        instance.mask = null;
        instance.maskTill = null;

        for (j = 0; j < resourceManager.m_data.DOMDocument.Text.length; j++)
        {
            if (resourceManager.m_data.DOMDocument.Text[j].charid == charId) 
            {
                instance.addText(resourceManager.m_data.DOMDocument.Text[j]);
            }
        }

        transformArray = transform.split(",");
        transformMat = new Snap.Matrix(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5]);
        instance.el.transform(transformMat);

        if (placeAfter && parseInt(placeAfter) !== 0) {
            afterMC = parentMC.getChildById(parseInt(placeAfter));

            if (afterMC.isMasked) {  //if masked add outside mask group
                afterMC.el.parent().before(instance.el);
            } else {
                afterMC.el.before(instance.el);
            }

        } else {
            parentEl.add(instance.el);
        }
    };

    this.addText = function (data) {

        var text,
            bbox,
            textBox,
            textX,
            textY,
            lineMode,
            fontSize,
            fontName,
            fontColor,
            textAlign,
            textAnchor,
            textIndent,
            textBaseline,
            textBounds,
            textStyles;

        textBox = instance.el.g();

        if (bounds) {
            textBounds = bounds.split(',');
        } else {
            textBounds = [0, 0, 200, 100];
        }

        lineMode = data.behaviour.lineMode;
        textAlign = data.paras[0].alignment;
        textBaseline = lineMode == 'single' ? 'central' : 'auto';
        fontSize = data.paras[0].textRun[0].style.fontSize;
        fontName = data.paras[0].textRun[0].style.fontName;
        fontColor = data.paras[0].textRun[0].style.fontColor;
        letterSpacing = data.paras[0].textRun[0].style.letterSpacing;

        if (textAlign == 'left') {
            textAnchor = 'start';
        } else if (textAlign == 'center') {
            textAnchor = 'middle';
        } else if (textAlign == 'right') {
            textAnchor = 'end';
        }

        textStyles = {
            'text-anchor': textAnchor,
            'dominant-baseline': textBaseline,
            'font-family': fontName,
            'font-size': fontSize,
            'letter-spacing': letterSpacing,
            'fill': fontColor
        };

        //rect
        if (data.behaviour.isBorderDrawn !== "false") {
            textRect = textBox.rect(textBounds[0], textBounds[1], textBounds[2], textBounds[3]);
            textRect.attr({
                stroke: 'black',
                fill: 'transparent'
            });
        }

        //make text and y
        if (lineMode == 'single') {
            text = textBox.text(0, 0, data.txt);
            textY = parseFloat(textBounds[1]) + (parseFloat(textBounds[3]) / 2);
        } else {
            text = instance.multiLine(textBox, data, textBounds, textStyles); 
            textY = parseFloat(textBounds[1]) - (parseFloat(data.paras[0].linespacing) * 2);
        }

        //x
        if (textAlign == 'left') {
            textX = parseFloat(textBounds[0]);
        } else {
            textX = parseFloat(textBounds[0]) + (parseFloat(textBounds[2]) / 2);
        }

        text.attr(textStyles);
        text.transform('translate(' + textX + ',' + textY + ')');
    };

    this.multiLine = function (textBox, data, textBounds, textStyles) {
        
        var string = data.txt,
            spans = [],
            chars = '',
            substr,
            tempTxt,
            boundsWidth = parseFloat(textBounds[2]),
            sp,
            bbox,
            i = 0;

        //break into spans
        while (i > -1) {
            chars += data.txt.charAt(i);
            tempTxt = textBox.text(0, 0, chars);
            tempTxt.attr(textStyles);

            bbox = tempTxt.getBBox();

            if (bbox.w > boundsWidth) {
                newIndex = chars.lastIndexOf(' ');
                substr = chars.slice(0, newIndex);
                spans.push(substr);
                i = i - (chars.length - substr.length) + 2;
                chars = '';
            } else {
                i += 1;
            }

            if (i >= data.txt.length) {
                substr = chars.slice(0, newIndex);
                spans.push(substr);
                i = -1;
            }

            tempTxt.remove();
        }

        text = textBox.text(0, 0, spans);
        sp = text.selectAll('tspan');
        sp.attr({
            'x': 0,
            'dy': bbox.h + parseFloat(data.paras[0].linespacing)
        });

        return text;
    };

    this.create();
};


var Shape = function (parentMC,resourceManager,charId,ObjectId,placeAfter,transform) {
    var instance = this,
        parentEl = parentMC.el;

    this.create = function () {
        var j,
            k,
            transformArray,
            transformMat;

        instance.el = parentMC.el.g();
        instance.id = ObjectId;
        instance.el.attr({'class': 'shape', 'token': instance.id});

        instance.children = [];
        instance.isMask = false;
        instance.isMasked = false;
        instance.mask = null;
        instance.maskTill = null;

        for (j = 0; j < resourceManager.m_data.DOMDocument.Shape.length; j++)
        {
            if (resourceManager.m_data.DOMDocument.Shape[j].charid == charId) 
            {
                for (k = 0; k < resourceManager.m_data.DOMDocument.Shape[j].path.length; k++)
                {
                    instance.addPath(j, k);   
                }
            }
        }

        transformArray = transform.split(",");
        transformMat = new Snap.Matrix(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5]);
        instance.el.transform(transformMat);

        if (placeAfter && parseInt(placeAfter) !== 0) {
            afterMC = parentMC.getChildById(parseInt(placeAfter));

            if (afterMC.isMasked) {  //if masked add outside mask group
                afterMC.el.parent().before(instance.el);
            } else {
                afterMC.el.before(instance.el);
            }
        } else {
            parentEl.add(instance.el); //TODO:: handle after
        }
    };

    this.addPath = function (j, k) {
        var clr,
            crlOpacity,
            shape1,
            path;

        shape = instance.el.path();
        resourcePath = resourceManager.m_data.DOMDocument.Shape[j].path[k];
        path = resourcePath.d;
        shape.attr({fill: 'transparent'});
        shape.attr({d: path});

        if(resourcePath.pathType == "Fill") {
            this.addFill(shape, resourcePath);
        } else if(resourcePath.pathType == "Stroke") {
            this.addStroke(shape, resourcePath);
        }
    };

    this.getFillColor = function (resoucePath) {
        var clr,
            r,
            g,
            b,
            a,
            colStr;

        clr = resourcePath.color;
        r = parseInt(clr.substring(1, 3), 16);
        g = parseInt(clr.substring(3, 5), 16);
        b = parseInt(clr.substring(5, 7), 16);
        a = resourcePath.colorOpacity;
        colStr = 'rgba(' + r + ',' + g + ',' + b + ',' + a + ')';

        return colStr;
    };

    this.getFillImage = function (resourceImg) {
        var p = 0,
            patternArray,
            mat,
            src,
            exists;

        patternArray = resourceImg.patternTransform.split(",");						
        p = 0;
        mat = new Snap.Matrix(patternArray[p],patternArray[p+1],patternArray[p+1],patternArray[p+3],patternArray[p+4],patternArray[p+5]);
        src = resourceImg.bitmapPath;
        
        exists = parentMC.el.paper.select('defs pattern image');
        
        if (exists) {
            if (exists.attr('href') == src) {	//check if image href matches as well as other props
                fillImage = exists.parent();
            } else {
                fillImage = newPattern(src, resourceImg, mat);
            }
        } else {
            fillImage = newPattern(src, resourceImg, mat);
        }

        return fillImage;
    };

    this.getFillGradient = function (grad, type, shape1) {
        var _x,
            _y,
            _x2,
            _y2,
            _fx,
            _fy,
            gradientString,
            gradientFill,
            i,
            rgb;

        if (type == 'linear') {
            _x = parseFloat(grad.x1);
            _y = parseFloat(grad.y1);
            _x2 = parseFloat(grad.x2);
            _y2 = parseFloat(grad.y2);
            gradientString = "L(";
            gradientString += _x + ", ";
            gradientString += _y + ", ";
            gradientString += _x2 + ", ";
            gradientString += _y2 + ")";
        } else {
            _x = (shape1.getBBox().x + shape1.getBBox().width / 2) + grad.cx / 10;
            _y = (shape1.getBBox().y + shape1.getBBox().height / 2) + grad.cy / 10;
            _fx = (shape1.getBBox().x + shape1.getBBox().width / 2) + parseFloat(grad.fx);
            _fy = (shape1.getBBox().y + shape1.getBBox().height / 2) + parseFloat(grad.fy);
            gradientString = "R("; 
            gradientString += _x + ", ";
            gradientString += _y + ", ";
            gradientString += grad.r + ", ";
            gradientString += _fx + ", ";
            gradientString += _fy + ")";
        }
        
        for (i = 0; i < grad.stop.length; i += 1)
        {	
            rgb = hexToRgb(grad.stop[i].stopColor);
            gradientString += 'rgba(' + rgb.r + ',' + rgb.g + ',' + rgb.b + ',' + grad.stop[i].stopOpacity + ')';		
            gradientString += ":";
            gradientString += grad.stop[i].offset;
            if (i !== grad.stop.length-1) {
                gradientString += "-";
            }		
        }

        gradientFill = parentMC.el.paper.gradient(gradientString);
        return gradientFill;
    };

    this.addFill = function (shape, resourcePath) {
        var fillColor,
            fillImage,
            fillGradient,
            img,
            grad;

        if(resourcePath.color)
        {
            fillColor = instance.getFillColor(resourcePath);
            shape.attr({fill: fillColor});
        }
        if(resourcePath.image)
        { 
            img = resourcePath.image;
            fillImage = instance.getFillImage(img);
            shape.attr({fill: fillImage});
        }
        if(resourcePath.linearGradient)
        {	
            grad = resourcePath.linearGradient;
            fillGradient = instance.getFillGradient(grad, 'linear');
            shape.attr({fill: fillGradient});
        }
        if(resourcePath.radialGradient)
        {	
            grad = resourcePath.radialGradient;
            fillGradient = instance.getFillGradient(grad, 'radial', shape);
            shape.attr({fill: fillGradient});
        }
    };

    this.addStroke = function (shape, resourcePath) {
        var r,
            g,
            b,
            a,
            colStr;

        if(resourcePath.color)
        {
            clr = resourcePath.color;

            r = parseInt(clr.substring(1, 3), 16);
            g = parseInt(clr.substring(3, 5), 16);
            b = parseInt(clr.substring(5, 7), 16);
            a = resourcePath.colorOpacity;

            colStr = 'rgba(' + r + ',' + g + ',' + b + ',' + a + ')';

            shape.attr({stroke: colStr, strokeWidth: resourcePath.strokeWidth});
        }
    };

    function hexToRgb(hex) {
        var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
        return result ? {
            r: parseInt(result[1], 16),
            g: parseInt(result[2], 16),
            b: parseInt(result[3], 16)
        } : null;
    }

    function newPattern(src, resourceImg, mat) {
        var image,
            pattern;

        image = instance.el.image(src);
        pattern = image.pattern(0, 0, resourceImg.width, resourceImg.height);
        pattern.attr({x: mat.e, y: mat.f});

        return pattern;
    }

    this.create();
};


var MovieClip = function (commandTimeline, s, resourceManager, objectID, name, transform) {
    var i,
        transformData,
        transformArray;
        parentEl = s.type == 'svg' ? s : s.el;  //parent is stage if svg

    if (objectID) {
      this.id = objectID;
    }

    if (name) {
      this.name = name;
    }

    this.el = parentEl.g();
    this.el.attr({'class': 'movieclip', 'token': this.id});
    this.transform = transform;

    this.m_timeline = commandTimeline;
    this.m_currentFrameNo = 0;
    this.m_frameCount = this.m_timeline.frameCount;

    this._scripts = {};
    this._labels = [];
    this.children = [];
    this.isMask = false;
    this.isMasked = false;
    this.mask = null;
    this.maskElement = null;
    this.maskTill = null;
    this.loops = true;
    this.playing = true;
    this.resourceManager = resourceManager;
    this.commandList = [];
    this.matrix = new Snap.Matrix();

    if (typeof(this.m_timeline.Label) !== 'undefined') {
      this._labels = this.m_timeline.Label;
    }

    if(this.transform !== undefined)
    {
        transformData = this.transform;
        transformArray = transformData.split(",");
        this.matrix = new Snap.Matrix(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5]);
        this.el.transform(this.matrix);
    }

    /*
    if (placeAfter && parseInt(placeAfter) !== 0) {
        afterMC = parentMC.getChildById(parseInt(placeAfter));

        if (afterMC.isMasked) {  //if masked add outside mask group
            afterMC.el.parent().before(this.el);
        } else {
            afterMC.el.before(this.el);
        }

    } else {
        parentEl.add(this.el);
    }
    */
};

MovieClip.prototype.addChild = function (child, placeAfter) {
  placeAfter = placeAfter ? placeAfter : 0;
  this.insertAtIndex(child, placeAfter);
}

MovieClip.prototype._addChild = function (child, placeAfter) {
  if (child.name) {
      this[child.name] = child;
  }

  if (placeAfter && parseInt(placeAfter) !== 0) {
      var afterMC = this.getChildById(parseInt(placeAfter));

      if (afterMC.isMasked) {  //if masked add outside mask group
          afterMC.el.parent().before(child.el);
      } else {
          afterMC.el.before(child.el);
      }

  } else {
      this.el.add(child.el);
  }
}

//manage children methods
MovieClip.prototype.getChildById = function (id) {
    var i;

    for (i = 0; i < this.children.length; i += 1) {
        if (this.children[i].id == id) {
            return this.children[i];
        }
    }

    return false;
};

MovieClip.prototype.getChildIndexById = function (id) {
    var i;

    for (i = 0; i < this.children.length; i += 1) {
        if (this.children[i].id == id) {
            return i;
        }
    }

    return false;
};

MovieClip.prototype.removeChildById = function (id) {
    var i;

    for (i = 0; i < this.children.length; i += 1) {
        if (this.children[i].id == id) {
            this.children.splice(i, 1);
            return;
        }
    }
};

MovieClip.prototype.swapChildIndex = function (id, placeAfter) {
    var i,
        child;

    for (i = 0; i < this.children.length; i += 1) {
        if (this.children[i].id == id) {
            child = this.children.splice(i, 1);
            break;
        }
    }

    for (i = 0; i < this.children.length; i += 1) {
        if (this.children[i].id == placeAfter) {
            this.children.splice(i + 1, 0, child[0]);
            break;
        }
    }
};

MovieClip.prototype.insertAtIndex = function (child, placeAfter) {
    var i;

    this._addChild(child, placeAfter);

    if (parseInt(placeAfter) === 0) {
        this.children.unshift(child);
    }

    for (i = 0; i < this.children.length; i += 1) {
        if (this.children[i].id == placeAfter) {
            this.children.splice(i + 1, 0, child);
            break;
        }
    }
};

MovieClip.prototype.containsMask = function () {
    var i;

    for (i = 0; i < this.children.length; i += 1) {
        if (this.children[i].isMask) {
            return true;
        }
    }

    return false;
};

MovieClip.prototype.getFrameLabels = function () {
  return this._labels;
}

MovieClip.prototype.getMatrix = function () {
  //TODO:: set to newM if exists

  if (this.matrix) {
    return this.matrix;
  } else {
    return new Snap.Matrix();
  }
}

MovieClip.prototype.getX = function () {
  var _x = 0;

  if (this.matrix) {
    _x = this.matrix.x();
  }

  return _x;
};

MovieClip.prototype.getY = function () {
  var _y = 0;

  if (this.matrix) {
    _y = this.matrix.y();
  }

  return _y;
};

//mouse event handlers
MovieClip.prototype.mouseover = function (cb) {
  this.el.mouseover(cb);
};

MovieClip.prototype.mouseout = function (cb) {
  this.el.mouseout(cb);
};

MovieClip.prototype.mousedown = function (cb) {
  this.el.mousedown(cb);
};

MovieClip.prototype.mousemove = function (cb) {
  this.el.mousemove(cb);
};

MovieClip.prototype.click = function (cb) {
  this.el.click(cb);
};

//script methods
MovieClip.prototype.executeFrameScript = function (script) {
  eval("(function () {" + script + "}).call(this);");
};

MovieClip.prototype.removeFrameScript = function (id) {
  delete this._scripts[id];
};

MovieClip.prototype.addFrameScript = function (id, script) {
  this._scripts[id] = script;
};

MovieClip.prototype.getFrame = function (num) {
  var i;

  for (i = 0; i < this.m_timeline.Frame.length; i += 1) {
    if (this.m_timeline.Frame[i].num == num) {
      return this.m_timeline.Frame[i];
    }
  }
};

MovieClip.prototype._checkLoop = function () {
  if(this.m_currentFrameNo == this.m_frameCount)
  {
      if (!this.loops) {
          return;
      }
      this._loop();
  }
};

//playback methods
MovieClip.prototype._loop = function () {
    var frame,
        commands,
        children,
        i,
        found,
        cmData,
        type;

    this.m_currentFrameNo = 0;

    frame = this.getFrame(this.m_currentFrameNo);

    if (!frame) {
      this.clearChildren();
      return;
    }

    //Get the commands for the first frame
    commands = frame.Command;

    // Iterate through all the elements in the display list
    // check if same instance exists in the first frame
    for (i = 0; i < this.children.length; i += 1) {

        found = false;
        child = this.children[i];

        for (c = 0; c < commands.length; ++c) {
            cmdData = commands[c];
            type = cmdData.cmdType;

            if (type == "Place") {
                if (parseInt(child.id) == parseInt(cmdData.objectId)) {
                    found = true;
                    break;
                }
            }
        }

        if (found === false) {
            command = new CMD.RemoveObjectCommand(child.id);
            this.commandList.push(command);
        }
    }
};

MovieClip.prototype.clearChildren = function () {
    var i,
        child,
        command;

    for (i = 0; i < this.children.length; i += 1) {
        child = this.children[i];
        command = new CMD.RemoveObjectCommand(child.id);
        this.commandList.push(command);
    }
};

MovieClip.prototype._animate = function () {
    var i;

    //if (!this.playing) {
    //  return;
    //}

    this.step_1_animTimeline();
    this.step_2_enterFrame();
    //this.step_3_addPending();
    this.step_4_frameConstructed();
    this.step_5_frameScripts();
    this.step_6_exitFrame();

    for(i = 0; i < this.children.length; i += 1)
    {
        if (this.children[i]._animate) {
            this.children[i]._animate();
        }
    }

    GP.purge();
};

MovieClip.prototype._runCommands = function (commands) {
  var c,
      cmdData,
      command,
      type,
      found;

  for(c = 0; c < commands.length; c += 1)
  {
      cmdData = commands[c];
      type = cmdData.cmdType;
      command = null;

      switch(type)
      {
          case "Place":

              found = this.getChildById(cmdData.objectId);

              if (!found) {
                  command = new CMD.PlaceObjectCommand(cmdData.charid, cmdData.objectId, cmdData.name, cmdData.placeAfter, cmdData.transformMatrix, cmdData.bounds);
                  this.commandList.push(command);
              } else {
                  command = new CMD.MoveObjectCommand(cmdData.objectId, cmdData.transformMatrix);
                  this.commandList.push(command);
                  command = new CMD.UpdateObjectCommand(cmdData.objectId, cmdData.placeAfter);
                  this.commandList.push(command);
              }

          break;
          case "Move":
              command = new CMD.MoveObjectCommand(cmdData.objectId, cmdData.transformMatrix);
              this.commandList.push(command);
          break;
          case "Remove":
              command = new CMD.RemoveObjectCommand(cmdData.objectId);
              this.commandList.push(command);
          break;
          case "UpdateZOrder":
              command = new CMD.UpdateObjectCommand(cmdData.objectId , cmdData.placeAfter);
              this.commandList.push(command);
          break;
          case "UpdateVisibility":
              command = new CMD.UpdateVisibilityCommand(cmdData.objectId , cmdData.visibility);
              this.commandList.push(command);
          break;
          case "UpdateColorTransform":
              command = new CMD.UpdateColorTransformCommand(cmdData.objectId , cmdData.colorMatrix);
              this.commandList.push(command);
          break;
          case "UpdateBlendMode":
            //  command = new CMD.UpdateBlendMode(cmdData.objectId , cmdData.blendMode);
            //  this.commandList.push(command);
          break;
          case "UpdateMask":
              command = new CMD.UpdateMaskCommand(cmdData.objectId , cmdData.maskTill);
              this.commandList.push(command);
          break;
          case "AddFrameScript":
            command = new CMD.AddFrameScriptCommand(cmdData.scriptId, cmdData.script);
            this.commandList.push(command);
          break;
          case "RemoveFrameScript":
            command = new CMD.RemoveFrameScriptCommand(cmdData.scriptId);
            this.commandList.push(command);
          break;
          case "SetFrameLabel":
            command = new CMD.SetFrameLabelCommand(cmdData.Name);
            this.commandList.push(command);
          break;
      }

  }

  if (this.containsMask) {
      command = new CMD.ApplyMaskCommand();
      this.commandList.push(command);
  }

  this.executeCommands(this.commandList, this.resourceManager);
}

//update timeline animations
MovieClip.prototype.step_1_animTimeline = function (seekMode, seekEnd) {

  if (typeof seekMode === "undefined") { seekMode = false; }
  if (typeof seekEnd === "undefined") { seekEnd = false; }

  var commands,
      frame;

  if (!this.playing) {
    return;
  }

  this.commandList = [];

  this._checkLoop();

  frame = this.getFrame(this.m_currentFrameNo);
  this.m_currentFrameNo++;

  if (!frame) {
    return;
  }

  commands = frame.Command;
  this._runCommands(commands);

};

MovieClip.prototype.step_2_enterFrame = function () {
  //dispatch enter frame event
  //trigger on children
};

MovieClip.prototype.step_3_addPending = function () {

};

MovieClip.prototype.step_4_frameConstructed = function () {
  //dispatch frame constructed event
  //trigger on children
};

MovieClip.prototype.step_5_frameScripts = function () {
  //trigger framescripts
  //trigger on children

  for (var i in this._scripts) {
    this.executeFrameScript(this._scripts[i]);
  }
};

MovieClip.prototype.step_6_exitFrame = function () {
  //dispatch exit frame event
  //trigger on children
};

MovieClip.prototype.play = function () {
  this.playing = true;
}

MovieClip.prototype.stop = function () {
  this.playing = false;
};

MovieClip.prototype.gotoAndStop = function (fr) {
  this._gotoAndPlayStop(fr, true);
};

MovieClip.prototype.gotoAndPlay = function (fr) {
  this._gotoAndPlayStop(fr, false);
};

MovieClip.prototype._gotoAndPlayStop = function (frame, bStop) {

  //TODO::handle labels
  if (typeof frame === "string") {
    var labels = this.getFrameLabels();
    var bFound = false;

    for (var i = labels.length - 1; i >= 0; i--) {
        if (frame === labels[i].name) {
            frame = parseInt(labels[i].frameNum) + 1;
            bFound = true;
            break;
        }
    }

    if (bFound === false) {
      return;
    }
  }

  //if frame number is invalid, don't do anything
  if (frame < 1 || frame > this.m_frameCount) {
    return;
  }

  // If we are already at the destination frame, don't do anythin
  if (frame == this.m_currentFrameNo) {
    if (bStop === false) {
        this.play();
    } else {
        this.stop();
    }
    return;
  }

  // First ensure that the timeline is in a good state to start jumping around
  //this.step_3_addPending(true);
  this.play();

  // Loop around if necessary
  if (frame < this.m_currentFrameNo) {
    var bSeekEnd = (frame == 1);
    this._loopAround(true, bSeekEnd);
    //this.step_3_addPending(!bSeekEnd);
  }

  while (this.m_currentFrameNo < frame) {
    var bSeekEnd = (frame == this.m_currentFrameNo);
    this.step_1_animTimeline(true, bSeekEnd);

    for(var i = 0; i < this.children.length; i += 1)
    {
        if (this.children[i].step_1_animTimeline) {
            this.children[i].step_1_animTimeline(true, bSeekEnd);
        }
    }

    //this.step_3_addPending(!bSeekEnd);
  }

  if (bStop === false) {
      this.play();
  } else {
      this.stop();
  }

  this.step_4_frameConstructed();
  this.step_5_frameScripts();
  this.step_6_exitFrame();
}

MovieClip.prototype._loopAround = function (seekMode, seekEnd) {
  if (typeof seekMode === "undefined") { seekMode = false; }
  if (typeof seekEnd === "undefined") { seekEnd = false; }

  this.commandList = [];
  this._checkLoop();
  this.m_currentFrameNo = 0;

  frame = this.getFrame(this.m_currentFrameNo);

  if (!frame) {
    return;
  }

  //Get the commands for the first frame
  commands = frame.Command;
  this._runCommands(commands);
}

MovieClip.prototype.executeCommands = function (commandList, resourceManager) {
    var i;

    for (i = 0; i < commandList.length ; i++)
    {
        if (commandList[i] !== undefined) {
             commandList[i].execute(this, resourceManager);
        }
    }
};

MovieClip.prototype.log = function () {
  if (this.id.indexOf('svg') > -1) { //only on main timeline
    var args = Array.prototype.slice.call(arguments);
    args.unshift(this.id.toUpperCase());
    //console.log.apply(console, args);
  }
}


	var CMD = {};


    //PlaceObjectCommand Class
    CMD.PlaceObjectCommand = function(charID, objectID, name, placeAfter, transform, bounds)
    {
        this.m_charID = charID;
        this.m_objectID = objectID;
				this.m_name = name;
        this.m_placeAfter = placeAfter;
        this.m_transform = transform;
        this.m_bounds = bounds;
    };

    //Execute function for PlaceObjectCommand
    CMD.PlaceObjectCommand.prototype.execute = function(parentMC, resourceManager)
    {

        var shape = resourceManager.getShape(this.m_charID),
            bitmap = resourceManager.getBitmap(this.m_charID),
            text = resourceManager.getText(this.m_charID),
            textObject,
            shapeObject,
            bmpObject,
            movieclipTimeline,
            movieclip;

        if(shape !== null && shape !== undefined)
        {
            shapeObject = new Shape(parentMC, resourceManager, this.m_charID, this.m_objectID, this.m_placeAfter, this.m_transform);
            parentMC.insertAtIndex(shapeObject, this.m_placeAfter);
        }
        else if(bitmap !== null && bitmap !== undefined)
        {
            bmpObject = new Bitmap(parentMC, resourceManager, this.m_charID, this.m_objectID, this.m_placeAfter, this.m_transform);
            parentMC.insertAtIndex(bmpObject, this.m_placeAfter);
        }
        else if (text !== null && text !== undefined)
        {
            textObject = new Text(parentMC, resourceManager, this.m_charID, this.m_objectID, this.m_placeAfter, this.m_transform, this.m_bounds);
            parentMC.insertAtIndex(textObject, this.m_placeAfter);
        }
        else
        {
            movieclipTimeline = resourceManager.getMovieClip(this.m_charID);
            if(movieclipTimeline)
            {
								movieclip = new MovieClip(movieclipTimeline, parentMC, resourceManager, this.m_objectID, this.m_name, this.m_transform);
								parentMC.insertAtIndex(movieclip, this.m_placeAfter);
                movieclip.play();
            }
        }
    };

    //MoveObjectCommand Class
    CMD.MoveObjectCommand = function(objectID, transform)
    {
        this.m_objectID = objectID;
        this.m_transform = transform;
    };

    //Execute function for PlaceObjectCommand
    CMD.MoveObjectCommand.prototype.execute = function(parentMC, resourceManager)
    {
        var transform,
            transformArray,
            transformMat;

        transform =  this.m_transform;
        transformArray = transform.split(",");
        transformMat = new Snap.Matrix(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5]);

        child = parentMC.getChildById(this.m_objectID);
				child.matrix = transformMat;
        child.el.transform(transformMat);
    };

    //UpdateObjectCommand Class
    CMD.UpdateObjectCommand = function(objectID, placeAfter)
    {
        this.m_objectID = objectID;
        this.m_placeAfter = placeAfter;
    };

    //Execute function for UpdateObjectCommand
    CMD.UpdateObjectCommand.prototype.execute = function(parentMC, resourceManager)
    {
        //console.log('update', this.m_objectID, this.m_placeAfter);
    };

    //RemoveObjectCommand Class
    CMD.RemoveObjectCommand = function(objectID)
    {
        this.m_objectID = objectID;
    };

    //Execute function for RemoveObjectCommand
    CMD.RemoveObjectCommand.prototype.execute = function(parentMC, resourceManager)
    {
        var child;

        child = parentMC.getChildById(this.m_objectID);
        child.el.remove();
        parentMC.removeChildById(this.m_objectID);
    };

    //UpdateVisbilityCommand Class
    CMD.UpdateVisibilityCommand = function(objectID,visibility)
    {
        this.m_objectID = objectID;
        this.m_visibility = visibility;
    };

    //Execute function for UpdateVisbilityCommand
    CMD.UpdateVisibilityCommand.prototype.execute = function(parentMC, resourceManager)
    {
        var child,
            visibleValue;

        child = parentMC.getChildById(this.m_objectID);
        visibleValue = this.m_visibility == "true" ? "visible" : "hidden";
        child.el.attr({'visibility': visibleValue});
    };

    CMD.UpdateMaskCommand = function (objectID,maskTill)
    {
        this.m_objectID = objectID;
        this.m_maskTill = maskTill;
    };

    function updateMaskContent(parentMC, child) {
        var def = child.maskElement,
            orig = parentMC.getChildById(child.id);

        def.clear();

        clone = orig.el.clone();
        clone.attr({visibility: 'visible'});

        def.append(clone);
    }

    CMD.UpdateMaskCommand.prototype.execute = function (parentMC, resourceManager)
    {
        //console.log('updatemask', this.m_objectID, this.m_maskTill);

        var maskConetent,
            mask,
            def,
            i;

        maskContent = parentMC.getChildById(this.m_objectID);
        maskContent.isMask = true;
        maskContent.maskTill = this.m_maskTill;

        mask = parentMC.el.mask();
        mask.attr('mask-type', 'alpha');

        //use clone to keep element in DOM for placement, just hide
        clone = maskContent.el.clone();
        clone.attr({visibility: 'visible'});

        def = mask.toDefs();
        def.append(clone);
        maskContent.maskElement = def;

        maskContent.el.attr({visibility: 'hidden'});
    };

    CMD.ApplyMaskCommand = function () {

    };

    /**
     * create group for mask and add masked content
     */
    CMD.ApplyMaskCommand.prototype.execute = function (parentMC, resourceManager) {
        var i,
            insideMask = false,
            currentMask = null,
            currentMaskEl = null,
            currentTill = null,
            currentMaskGroup;

        for (i = 0; i < parentMC.children.length; i += 1) {
            child = parentMC.children[i];

            if (child.isMask) {

                updateMaskContent(parentMC, child); //mask needs to update

                insideMask = true;
                currentMask = child;
                currentMaskEl = child.maskElement;
                currentTill = child.maskTill;
                currentMaskGroup = parentMC.el.g();
                currentMaskGroup.attr({'class': 'maskGroup'});
                child.el.after(currentMaskGroup);
                currentMaskGroup.attr({mask: currentMaskEl});

                GP.addEmpty(currentMaskGroup);
                GP.addRef(currentMaskEl, [currentMaskGroup]);

                if (child.id == child.maskTill) {
                    insideMask = false;
                }

            } else {

                if (insideMask) {

                    currentMaskGroup.prepend(child.el);
                    child.isMasked = true;
                    child.mask = currentMask.id;

                    if (child.id == currentTill) {
                        insideMask = false;
                        currentMask = null;
                        currentTill = null;
                    }
                }
            }
        }
    };

    CMD.UpdateColorTransformCommand = function (objectID, colorMatrix)
    {
        this.m_objectID = objectID;
        this.m_colorMatrix = colorMatrix;
    };

    CMD.UpdateColorTransformCommand.prototype.execute = function (parentMC, resourceManager)
    {
        var child,
            matrix;

        child = parentMC.getChildById(this.m_objectID);
        matrix = this.m_colorMatrix.split(',', 7);
        child.el.attr({opacity: parseFloat(matrix[6])}); //currently only alpha
    };

		CMD.AddFrameScriptCommand = function (scriptID, script)
		{
			this.m_scriptID = scriptID;
			this.m_script = script;
		};

		CMD.AddFrameScriptCommand.prototype.execute = function (parentMC, resourceManager)
		{
			parentMC.addFrameScript(this.m_scriptID, this.m_script);
		};

		CMD.RemoveFrameScriptCommand = function (scriptID)
		{
			this.m_scriptID = scriptID;
		};

		CMD.RemoveFrameScriptCommand.prototype.execute = function (parentMC, resourceManager)
		{
			parentMC.removeFrameScript(this.m_scriptID);
		};

		CMD.SetFrameLabelCommand = function (name)
		{
			this.m_labelName = name;
		};

		CMD.SetFrameLabelCommand.prototype.execute = function (parentMC, resourceManager)
		{

		};


var ResourceManager = function (data) {
    var id;

    this.m_shapes = [];
    this.m_movieClips = [];
    this.m_bitmaps = [];
    this.m_text = [];
    this.m_data = data;
    
    //Parse shapes and movieClips	
    for(var shapeIndex =0; shapeIndex < this.m_data.DOMDocument.Shape.length; shapeIndex++)
    {
        id = this.m_data.DOMDocument.Shape[shapeIndex].charid;
        var shapeData = this.m_data.DOMDocument.Shape[shapeIndex];
        this.m_shapes[id] = shapeData;
    }

    for(var bitmapIndex =0; bitmapIndex < this.m_data.DOMDocument.Bitmaps.length; bitmapIndex++)
    {
        id = this.m_data.DOMDocument.Bitmaps[bitmapIndex].charid;
        var bitmapData = this.m_data.DOMDocument.Bitmaps[bitmapIndex];
        this.m_bitmaps[id] = bitmapData;
    }

    for(var textIndex =0; textIndex < this.m_data.DOMDocument.Text.length; textIndex++)
    {
        id = this.m_data.DOMDocument.Text[textIndex].charid;
        var textData = this.m_data.DOMDocument.Text[textIndex];
        this.m_text[id] = textData;
    }
    
    if(this.m_data.DOMDocument.Timeline !== undefined)
    {
        for(var movieClipIndex =0; movieClipIndex < this.m_data.DOMDocument.Timeline.length - 1; movieClipIndex++)
        {
            id = this.m_data.DOMDocument.Timeline[movieClipIndex].charid;
            var movieClipData = this.m_data.DOMDocument.Timeline[movieClipIndex];
            this.m_movieClips[id] = movieClipData;
        }
    }

    console.log(this.m_data.DOMDocument);
};

//Member functions
ResourceManager.prototype.getShape = function(id) {
    return this.m_shapes[id];
};

ResourceManager.prototype.getMovieClip = function(id) {
    return this.m_movieClips[id];
};

ResourceManager.prototype.getBitmap = function(id) {
    return this.m_bitmaps[id];
};

ResourceManager.prototype.getText = function(id) {
    return this.m_text[id];
};


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
        color = '#008460';

    instance.version = '1.2.1';

    msg = 'Snap.svg Animator v' + instance.version;
    console.log("%c" + msg, "color:" + color + ";font-weight:bold;");

    params = params|| {};
    fps = fps || 24;
    w = w || 100;
    h = h || 100;

    autoplay = params.autoplay || true;
    playing = autoplay;

    instance.debug = false;

    SVGAnim.prototype.toString = function () {
        return msg;
    };

    instance.MovieClip = MovieClip;

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
        playing = false;
        window.addEventListener('keydown', handleKeyDown);
    }

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
      instance.mc.stop();
      playing = false;
    };

    this.setLoop = function (l) {
        instance.mc.loops = l;
    };

    function interval() {
        instance.mc._animate();

        clearTimeout(cbk);
        if (playing) {
            cbk = setTimeout(interval, 1000 / fps);
        }
    }

    function handleKeyDown(e) {
        switch (e.keyCode) {
          case 39:
           interval();
          break;
          case 32:
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
    window.SVGAnim = SVGAnim;
    return SVGAnim;
}(window || this));
