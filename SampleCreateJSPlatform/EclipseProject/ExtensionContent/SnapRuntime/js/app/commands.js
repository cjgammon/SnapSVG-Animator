/*global define $ requestAnimationFrame*/

define(function (require) {
	
	var CMD = {};
    
    require(['app/shape', 'app/movieclip'], function (Shape, MovieClip) {

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
            
            var shape = resourceManager.getShape(this.m_charID),
                bitmap = resourceManager.getBitmap(this.m_charID),
                text = resourceManager.getText(this.m_charID),
                movieclipTimeline,
                movieclip,
                children,
                childMC,
                index;

            if(shape !== null && shape !== undefined)
            {
                shapeObject = new Shape(parentMC, resourceManager, this.m_charID, this.m_objectID, this.m_placeAfter, this.m_transform);
                parentMC.children.push(shapeObject);
            } 
            else if(bitmap !== null && bitmap !== undefined)
            {
                //Utils.CreateBitmap(root, parentMC, resourceManager, this.m_charID, this.m_objectID, this.m_placeAfter, this.m_transform);
            }
            else if (text !== null && text !== undefined) 
            {
                //Utils.CreateText(root, parentMC, resourceManager, this.m_charID, this.m_objectID, this.m_placeAfter, this.m_transform);
            }
            else
            {
                // Movie clip logic starts here
                            

                    //Create a  MC
                    //childMC = parentMC.g();
                    //childMC.attr({class: 'movieclip', token: this.m_objectID});
                    
                    /*
                    if(this.m_placeAfter !== 0)
                    {
                        children = parentMC.selectAll('[token="' + this.m_placeAfter + '"]');
                        for (i = 0; i < children.length; i += 1) {
                            if (children[i].parent().id == parentMC.id) { //ensure child of current movie clip
                                children[i].before(childMC);
                                break;
                            }
                        }
                    }
                    else
                    {
                        parentMC.add(childMC);
                    }
                    */

                movieclipTimeline = resourceManager.getMovieClip(this.m_charID);
                if(movieclipTimeline)
                {
                    movieclip = new MovieClip(parentMC, movieclipTimeline, resourceManager, this.m_objectID, this.m_placeAfter, this.m_transform);
                    parentMC.children.push(movieclip);
                    movieclip.play(resourceManager);
                }
            }

        };

        //MoveObjectCommand Class
        CMD.MoveObjectCommand = function(objectID, transform) 
        {
            this.m_objectID = objectID;
            //this.m_placeAfter = placeAfter;
            this.m_transform = transform;	
        };

        //Execute function for PlaceObjectCommand
        CMD.MoveObjectCommand.prototype.execute = function(stage, resourceManager)
        {
            var parentMC,
                transform,
                transformArray,
                transformMat,
                children,
                i;
                
            parentMC = stage.el;
            transform =  this.m_transform;
            transformArray = transform.split(",");
            transformMat = new Snap.Matrix(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5]);

            if(parentMC != undefined)
            {
                children = parentMC.selectAll('[token="' + this.m_objectID + '"]');
                
                for (i = 0; i < children.length; i += 1) {
                    if (children[i].parent().id == parentMC.id) { //ensure child of current movie clip
                        children[i].transform(transformMat);
                    }
                }		
            }
            
        };

        //UpdateObjectCommand Class
        CMD.UpdateObjectCommand = function(objectID, placeAfter) 
        {
            this.m_objectID = objectID;
            this.m_placeAfter = placeAfter;
        };

        //Execute function for UpdateObjectCommand
        CMD.UpdateObjectCommand.prototype.execute = function(timelineAnimator, resourceManager)
        {
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
        CMD.RemoveObjectCommand.prototype.execute = function(stage, resourceManager)
        {
            var parentMC = stage.el,
                child;
            
            if(parentMC != undefined)
            {
                children = parentMC.selectAll('[token="' + this.m_objectID + '"]');
                
                for (i = 0; i < children.length; i += 1) {
                    if (children[i].parent() == parentMC) { //ensure child of current movie clip
                        children[i].remove();
                    }
                }
            }	
        };

        //UpdateVisbilityCommand Class
        CMD.UpdateVisibilityCommand = function(objectID,visibility) 
        {
            this.m_objectID = objectID;	
            this.m_visibilty = visibility;
        };

        //Execute function for UpdateVisbilityCommand
        CMD.UpdateVisibilityCommand.prototype.execute = function(timelineAnimator, resourceManager)
        {
            /*
            var parentMC = timelineAnimator.m_targetMC;
            if(parentMC != undefined)
            {
                //Remove the targetMC
                var index,visibleBool;							
                for(var index=0; index<parentMC.children.length; index++)
                {
                    if(parentMC.children[index].id == parseInt(this.m_objectID))
                    {
                        if(this.m_visibilty == "true")
                            visibleBool = true;
                            else
                            visibleBool = false;

                        parentMC.getChildAt(index).visible = visibleBool;

                    }				
                }		
            }
            */	
        };
        
        CMD.UpdateMaskCommand = function (objectID,maskTill) 
        {
            this.m_objectID = objectID;
            this.m_maskTill = maskTill;
        };

        CMD.UpdateMaskCommand.prototype.execute = function (stage, resourceManager) 
        {
            var parentMC = stage.el,
                root = stage.root,
                mask,
                maskContent,
                masked,
                i,
                def,
                clone;
            
            if(parentMC != undefined)
            {
                maskContent = parentMC.select('[token="' + this.m_objectID + '"]');
                mask = root.mask();
                mask.attr('mask-type', 'alpha');
                mask.append(maskContent);

                def = mask.toDefs();
                
                for (i = parseInt(this.m_maskTill); i > parseInt(this.m_objectID); i -= 1) {
                    //clone = def.clone(); //issue with reusing def ??
                    masked = parentMC.select('[token="' + i + '"]');
                    masked.attr({mask: def});
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
            //var parentEl = parentMC.el,
                //object;

            //if (parentMC != undefined) {
                //object = parentMC.select('[token="' + this.m_objectID + '"]');
                //matrix = this.m_colorMatrix.split(',', 7);
                //object.attr({opacity: parseFloat(matrix[6])}); //currently only alpha
            //}
        };

    });

	return CMD;
	
});
