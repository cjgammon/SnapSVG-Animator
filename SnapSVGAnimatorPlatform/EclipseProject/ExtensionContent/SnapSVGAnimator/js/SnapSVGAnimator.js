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
            afterMC.el.before(instance.el);
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
            afterMC.el.before(instance.el);
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
            afterMC.el.before(instance.el);
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


var MovieClip = function (parentMC, commandTimeline, resourceManager, objectID, placeAfter, transform) {
    var i,
        transformMat,
        transformData,
        transformArray,
        afterEl,
        parentEl = parentMC.type == 'svg' ? parentMC : parentMC.el;  //parent is stage if svg

    if (objectID) {
        this.id = objectID;
    }
    this.el = parentEl.g();
    this.el.attr({'class': 'movieclip', 'token': this.id});
    this.transform = transform;

    this.m_timeline = commandTimeline;
    this.m_currentFrameNo = 0;
    //this.m_frameCount = this.m_timeline.Frame.length;
    this.m_frameCount = this.m_timeline.frameCount;

    this.children = [];
    this.isMask = false;
    this.isMasked = false;
    this.mask = null;
    this.maskElement = null;
    this.maskTill = null;
    this.loops = true;

    if(this.transform !== undefined) 
    {
        transformData = this.transform;
        transformArray = transformData.split(",");
        transformMat = new Snap.Matrix(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5]);
        this.el.transform(transformMat);
    }

    if (placeAfter && parseInt(placeAfter) !== 0) {
        afterMC = parentMC.getChildById(parseInt(placeAfter));
        afterMC.el.before(this.el);
    } else {
        parentEl.add(this.el);         
    }
};

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

MovieClip.prototype.loop = function (commandList) {
    var frame,
        commands,
        children,
        i,
        found,
        cmData,
        type;

    this.m_currentFrameNo = 0;

    /*
    frame = this.m_timeline.Frame[this.m_currentFrameNo];	
    */
    ////////////////
    for (i = 0; i < this.m_timeline.Frame.length; i += 1) {
        if (parseInt(this.m_timeline.Frame[i].num) == this.m_currentFrameNo) {
            frame = this.m_timeline.Frame[i];
            break;
        } else if (i >= this.m_timeline.Frame.length - 1) {
            return;
        }
    }
    ////////////////
    if (!frame) {
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
            commandList.push(command);
        }
    }
};

MovieClip.prototype.play = function (resourceManager) {
    var commandList = [],
        frame,
        i,
        c,
        found,
        commands,
        command,
        cmdData,
        type;

    //play movieclips
    for(i = 0; i < this.children.length; i += 1)
    {
        if (this.children[i].play) {
            this.children[i].play(resourceManager);
        }
    }

    //check to handle looping of movieclip
    if(this.m_currentFrameNo == this.m_frameCount) 
    {
        if (!this.loops) {
            return;
        }
        this.loop(commandList);
    }

    /*
    frame = this.m_timeline.Frame[this.m_currentFrameNo];
    */
    ////////////////
    for (i = 0; i < this.m_timeline.Frame.length; i += 1) {
        if (parseInt(this.m_timeline.Frame[i].num) == this.m_currentFrameNo) {
            frame = this.m_timeline.Frame[i];
            break;
        } else if (i >= this.m_timeline.Frame.length - 1) {
            this.m_currentFrameNo += 1;
            return;
        }
    }
    ////////////////
    if (!frame) {
        return;
    }

    commands = frame.Command;
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
                    command = new CMD.PlaceObjectCommand(cmdData.charid, cmdData.objectId, cmdData.placeAfter, cmdData.transformMatrix, cmdData.bounds);
                    commandList.push(command);
                } else {
                    command = new CMD.MoveObjectCommand(cmdData.objectId, cmdData.transformMatrix);
                    commandList.push(command);
                    command = new CMD.UpdateObjectCommand(cmdData.objectId, cmdData.placeAfter);
                    commandList.push(command);
                }

            break;
            case "Move":
                command = new CMD.MoveObjectCommand(cmdData.objectId, cmdData.transformMatrix);
                commandList.push(command);
            break;
            case "Remove":
                command = new CMD.RemoveObjectCommand(cmdData.objectId);
                commandList.push(command);
            break;
            case "UpdateZOrder":
                command = new CMD.UpdateObjectCommand(cmdData.objectId , cmdData.placeAfter);
                commandList.push(command);
            break;
            case "UpdateVisibility":
                command = new CMD.UpdateVisibilityCommand(cmdData.objectId , cmdData.visibility);
                commandList.push(command);
            break;
            case "UpdateColorTransform":
                command = new CMD.UpdateColorTransformCommand(cmdData.objectId , cmdData.colorMatrix);
                commandList.push(command);
            break;
            case "UpdateMask":
                command = new CMD.UpdateMaskCommand(cmdData.objectId , cmdData.maskTill);
                commandList.push(command);
            break;
        }

    }

    if (this.containsMask) {
        command = new CMD.ApplyMaskCommand();
        commandList.push(command);
    }

    for (i = 0; i < commandList.length ; i++)
    {
        if (commandList[i] !== undefined) {
             commandList[i].execute(this, resourceManager);
        }
    }

    this.m_currentFrameNo++;

    GP.purge();
};



	var CMD = {};
    

    //PlaceObjectCommand Class
    CMD.PlaceObjectCommand = function(charID, objectID, placeAfter, transform, bounds) 
    {
        this.m_charID = charID;
        this.m_objectID = objectID;
        this.m_placeAfter = placeAfter;
        this.m_transform = transform;
        this.m_bounds = bounds;
    };

    //Execute function for PlaceObjectCommand
    CMD.PlaceObjectCommand.prototype.execute = function(parentMC, resourceManager)
    {
        //console.log('place', this.m_objectID, this.m_placeAfter);

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
                movieclip = new MovieClip(parentMC, movieclipTimeline, resourceManager, this.m_objectID, this.m_placeAfter, this.m_transform);
                parentMC.insertAtIndex(movieclip, this.m_placeAfter);
                movieclip.play(resourceManager);
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
        //console.log('move', this.m_objectID);

        var transform,
            transformArray,
            transformMat;
            
        transform =  this.m_transform;
        transformArray = transform.split(",");
        transformMat = new Snap.Matrix(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5]);

        child = parentMC.getChildById(this.m_objectID);
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
        autoplay,
        cbk;

    params = params|| {};
    fps = fps || 24;
    w = w || 100;
    h = h || 100;
    
    autoplay = params.autoplay;    

    SVGAnim.prototype.toString = function () {
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

    if (autoplay) {
        instance.play();
    } else {
        loop();
    }

}

    window.SVGAnim = SVGAnim;
    return SVGAnim;
}(window || this));
