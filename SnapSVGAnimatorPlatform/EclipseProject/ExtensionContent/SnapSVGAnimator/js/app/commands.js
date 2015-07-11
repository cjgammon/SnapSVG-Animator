/*global define $ requestAnimationFrame*/

define(function (require) {
	
	var CMD = {};
    
    require(['app/shape', 'app/bitmap', 'app/movieclip'], function (Shape, Bitmap, MovieClip) {

        //PlaceObjectCommand Class
        CMD.PlaceObjectCommand = function(charID, objectID, placeAfter, transform) 
        {
            this.m_charID = charID;
            this.m_objectID = objectID;
            this.m_placeAfter = placeAfter;
            this.m_transform = transform;
        };

        //Execute function for PlaceObjectCommand
        CMD.PlaceObjectCommand.prototype.execute = function(parentMC, resourceManager)
        {
            //console.log('place', this.m_objectID, this.m_placeAfter);
            var shape = resourceManager.getShape(this.m_charID),
                bitmap = resourceManager.getBitmap(this.m_charID),
                text = resourceManager.getText(this.m_charID),
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
                //Utils.CreateText(root, parentMC, resourceManager, this.m_charID, this.m_objectID, this.m_placeAfter, this.m_transform);
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
            //parentMC.swapChildIndex(this.m_objectID, this.m_placeAfter);
            /*
            var parentMC = timelineAnimator.s;

            if(parentMC != undefined)
            {
                //Change the Z order of the targetMC
                var index;							
                for(var indexz=0; indexz<parentMC.children.length; indexz++)
                {
                    if(parentMC.children[indexz].id == parseInt(this.m_objectID))
                    {
                        var child = parentMC.getChildAt(indexz);
                        if(this.m_placeAfter != 0)
                        {					
                            for(var index=0; index<parentMC.children.length; index++)
                            {
                                if(parentMC.children[index].id == parseInt(this.m_placeAfter))
                                {
                                    //child.addChildAt(childMC,index);
                                    parentMC.setChildIndex(child,index - 1)
                                    break;
                                }				
                            }
                        }				
                        break;
                    }				
                }		
            }
            */
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

                    insideMask = true;
                    currentMask = child;
                    currentMaskEl = child.maskElement;
                    currentTill = child.maskTill;
                    currentMaskGroup = parentMC.el.g();
                    currentMaskGroup.attr({'class': 'maskGroup'});
                    child.el.after(currentMaskGroup);
                    currentMaskGroup.attr({mask: currentMaskEl});

                    if (child.id == child.maskTill) {
                        insideMask = false;
                    }
                    //TODO:: add group to pool of items to remove

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

    });

	return CMD;
	
});
