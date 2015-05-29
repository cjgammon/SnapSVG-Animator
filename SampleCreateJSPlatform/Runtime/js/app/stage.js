/*global define $ requestAnimationFrame*/

define(function (require) {
	
	var MovieClip,
		Utils = require('app/utils');
	
	require('tweenmax');
	
	MovieClip = function (root, el, commandTimeline, resourceManager, transform) {
		var i,
			transformMat;

		this.root = root;
    	this.el = el;
		this.m_transform = transform;

		this.m_timeline = commandTimeline;
		this.m_currentFrameNo = 0;
		this.m_frameCount = this.m_timeline.Frame.length;
		this.m_children = [];

		if(this.m_transform !== undefined) 
		{
			//Apply the transformation on the parent MC
			var transformData =  this.m_transform;
			var transformArray = transformData.split(",");
			transformMat = new Snap.Matrix(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5]);
			this.el.transform(transformMat.toTransformString());
		}
		
	}

	MovieClip.prototype.play = function (resourceManager) {
		var children = this.m_children;
    	var commandList = [];

		for(var i=0; i<children.length; ++i)
		{
			children[i].play(resourceManager);
		}

		// Handle Looping
		if(this.m_currentFrameNo == this.m_frameCount) {
			this.m_currentFrameNo = 0;

	    	var frame = this.m_timeline.Frame[this.m_currentFrameNo];	

	    	if (!frame) {
	    		return;
	    	}

	    	//Get the commands for the first frame
	    	var commands = frame.Command;	

	    	// Iterate through all the elements in the display list (maintained by CreateJS) and 
		    // check if same instance exists in the first frame 

		    var children = this.root.selectAll('*');

		    for (var i = 0; i < children.length; i += 1) {
		    	if (children[i].parent().id == this.el.id) {

		    		var found = false;
		        	var elementId = children[i].attr('token');

		        	for (var c = 0; c < commands.length; ++c) {
			            var cmdData = commands[c];
			            var type = cmdData.cmdType;

			            if (type == "Place") {
			                if (parseInt(elementId) == parseInt(cmdData.objectId)) {
			                    found = true;
			                    break;
			                }
			            }
			        }

			        if (found == false) {
			            command = new RemoveObjectCommand(elementId);
			            commandList.push(command);
			        }

		    	}
		    }
		}

		//run commands
		var frame = this.m_timeline.Frame[this.m_currentFrameNo];
	  	if (!frame) {
	  		return;
    	}
		var commands = frame.Command;

		for(var c=0; c<commands.length; ++c)
		{
			var cmdData = commands[c];
			var type = cmdData.cmdType;
			var command = undefined;

			switch(type)
			{
				case "Place":

					var found = false;
					var items = this.root.selectAll('[token="' + cmdData.objectId + '"]');
					for (var i = 0; i < items.length; i += 1) {
						if (items[i].parent() == this.el || items[i].parent().type == 'mask') { //this is sort of a hack, i think there should be a better way of managing masks
							found = true;
						}
					}

			        if (!found) {
			            command = new PlaceObjectCommand(cmdData.charid, cmdData.objectId, cmdData.placeAfter, cmdData.transformMatrix);
			            commandList.push(command);
			        } else {
	                    // It is already present (Possible for looping case)
			            command = new MoveObjectCommand(cmdData.objectId, cmdData.transformMatrix);
			            commandList.push(command);
			            command = new UpdateObjectCommand(cmdData.objectId, cmdData.placeAfter);
			            commandList.push(command);
			        }

				break;
				case "Move":
					command = new MoveObjectCommand(cmdData.objectId, cmdData.transformMatrix);
					commandList.push(command);
				break;
				case "Remove":
					command = new RemoveObjectCommand(cmdData.objectId);
					commandList.push(command);
				break;
				case "UpdateZOrder":
					command = new UpdateObjectCommand(cmdData.objectId , cmdData.placeAfter);
					commandList.push(command);
				break;
				case "UpdateVisibility":
					command = new UpdateVisibilityCommand(cmdData.objectId , cmdData.visibility);
					commandList.push(command);
				break;
				case "UpdateColorTransform":
					command = new UpdateColorTransformCommand(cmdData.objectId , cmdData.colorMatrix);
					commandList.push(command);
				break;
				case "UpdateMask":
					console.log('update mask');
					command = new UpdateMaskCommand(cmdData.objectId , cmdData.maskTill);
					commandList.push(command);
				break;
			}

		}
	
		for (var i = 0; i < commandList.length ; i++)
		{
	  		//Execute it
			if (commandList[i] !== undefined) {
			     commandList[i].execute(this, resourceManager);
			}
		}

		//Increment the current frame no
		this.m_currentFrameNo++;

		this.cleanupUnusedDefs(); //fixes issue  with masks looping
	}
	
	/**
	* clean up unused defs
	* TODO:: perf hit, optimize
	**/
	MovieClip.prototype.cleanupUnusedDefs = function () {
		var that = this,
			defs = this.el.selectAll('defs>*'),
			j,
			i,
			id,
			toRemove;
		
		//iterate all objects
		function loopObjects(_id) {
			var objects = that.el.selectAll('*'),
				count = 0,
				mask,
				fill;
							
			for (i = 0; i < objects.length; i += 1) {
				mask = objects[i].attr('mask').replace('url(#', '').replace(')', '');
				fill = objects[i].attr('fill');
								
				if (fill.indexOf('#') > -1) {
					fill = fill.replace('url(#', '').replace(')', '');
				} else {
					fill = '';
				}
				
				if (mask !== _id && fill !== _id) {
					count += 1;
				}
			}
			
			return count == objects.length;
		}
		
		//loop through defs
		for (j = 0; j < defs.length; j += 1) {
			id = defs[j].attr('id');

			if (!id) {
				continue;
			}
						
			toRemove = loopObjects(id);
			
			if (toRemove) {
				defs[j].remove();
			}
		}
		

		//clear all groups moved to defs
		defGroups = this.el.selectAll('defs>g');
		for (i = 0; i < defGroups.length; i += 1) {
			defGroups[i].remove();
		}

		//clear all unused masks
		masks = this.el.selectAll('defs>mask');
		for (i = 0; i < masks.length; i += 1) {
			if (!masks[i].attr('id')) {
				masks[i].remove();
			}
		}
	}
	

	//PlaceObjectCommand Class
	var PlaceObjectCommand = function(charID, objectID, placeAfter, transform) 
	{
		this.m_charID = charID;
		this.m_objectID = objectID;
		this.m_placeAfter = placeAfter;
		this.m_transform = transform;
	}

	//Execute function for PlaceObjectCommand
	PlaceObjectCommand.prototype.execute = function(stage, resourceManager)
	{
		
		var shape = resourceManager.getShape(this.m_charID),
			bitmap = resourceManager.getBitmap(this.m_charID),
			text = resourceManager.getText(this.m_charID),
			parentMC = stage.el,
			root = stage.root,
			movieclipTimeline,
			movieclip,
			children,
			i,
			childMC,
			index;
		
		if(shape !== null && shape !== undefined)
		{
			Utils.CreateShape(root, parentMC, resourceManager, this.m_charID, this.m_objectID, this.m_placeAfter, this.m_transform);
		} 
		else if(bitmap !== null && bitmap !== undefined)
		{
			Utils.CreateBitmap(root, parentMC, resourceManager, this.m_charID, this.m_objectID, this.m_placeAfter, this.m_transform);
		}
		else if (text !== null && text !== undefined) 
		{
			Utils.CreateText(root, parentMC, resourceManager, this.m_charID, this.m_objectID, this.m_placeAfter, this.m_transform);
		}
		else
		{
			// Movie clip logic starts here
			movieclipTimeline = resourceManager.getMovieClip(this.m_charID);
						
			if(parentMC != undefined)
			{

				//Create a  MC
				childMC = parentMC.g();
				childMC.attr({class: 'movieclip', token: this.m_objectID});
				
				if(this.m_placeAfter != 0)
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

				//Create a corresponding TimelineAnimator
				if(movieclipTimeline)
				{
					movieclip = new MovieClip(root, childMC, movieclipTimeline, resourceManager, this.m_transform);
					stage.m_children.push(movieclip);
					movieclip.play(resourceManager);
				}
			}
		}

	}

	//MoveObjectCommand Class
	var MoveObjectCommand = function(objectID, transform) 
	{
		this.m_objectID = objectID;
		//this.m_placeAfter = placeAfter;
		this.m_transform = transform;	
	}

	//Execute function for PlaceObjectCommand
	MoveObjectCommand.prototype.execute = function(stage, resourceManager)
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
					children[i].transform(transformMat.toTransformString());
				}
			}		
		}
		
	}

	//UpdateObjectCommand Class
	var UpdateObjectCommand = function(objectID, placeAfter) 
	{
		this.m_objectID = objectID;
		this.m_placeAfter = placeAfter;
	}

	//Execute function for UpdateObjectCommand
	UpdateObjectCommand.prototype.execute = function(timelineAnimator, resourceManager)
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
	}

	//RemoveObjectCommand Class
	var RemoveObjectCommand = function(objectID) 
	{
		this.m_objectID = objectID;	
	}

	//Execute function for RemoveObjectCommand
	RemoveObjectCommand.prototype.execute = function(stage, resourceManager)
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
	}

	//UpdateVisbilityCommand Class
	var UpdateVisibilityCommand = function(objectID,visibility) 
	{
		this.m_objectID = objectID;	
		this.m_visibilty = visibility;
	}

	//Execute function for UpdateVisbilityCommand
	UpdateVisibilityCommand.prototype.execute = function(timelineAnimator, resourceManager)
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
	}
	
	var UpdateMaskCommand = function (objectID,maskTill) 
	{
		this.m_objectID = objectID;
		this.m_maskTill = maskTill;
	}

	UpdateMaskCommand.prototype.execute = function (stage, resourceManager) 
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
	}

	var UpdateColorTransformCommand = function (objectID, colorMatrix)
	{
		this.m_objectID = objectID;
		this.m_colorMatrix = colorMatrix;
	}

	UpdateColorTransformCommand.prototype.execute = function (stage, resourceManager) 
	{
		var parentMC = stage.el,
			object;

		if (parentMC != undefined) {
			object = parentMC.select('[token="' + this.m_objectID + '"]');
			matrix = this.m_colorMatrix.split(',', 7);
			object.attr({opacity: parseFloat(matrix[6])}); //currently only alpha
		}
	}

	return MovieClip;
	
});