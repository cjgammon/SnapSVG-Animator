
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
