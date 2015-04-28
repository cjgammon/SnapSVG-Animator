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

		//Timeline is a collection of frames
		//Frame is a collection of Command Objects
		this.m_ctimeline = commandTimeline;
		this.m_timeline = new TimelineMax({
			useFrames: true, 
			paused: true,
			onRepeat: this.repeat.bind(this), 
			onUpdate: this.runCommands.bind(this), 
			onUpdateParams: [resourceManager]
			});
		this.m_currentFrameNo = this.m_timeline.time();

		this.m_frameCount = this.m_ctimeline.Frame.length;
		this.m_children = [];
		if(this.m_transform !== undefined) 
		{
			//Apply the transformation on the parent MC
			var transformData =  this.m_transform;
			var transformArray = transformData.split(",");
			transformMat = new Snap.Matrix(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5]);
			console.log('matrix', transformMat, transformMat.toTransformString());
			this.el.transform(transformMat.toTransformString());
		}
		
		this.m_timeline.add(function () {}, this.m_frameCount + 1);
	}
	
	//TODO:: should remove associated defs upon removing elements
	//TODO:: should also check if asset/def exists when adding new one to reuse instead of adding/removing
	
	MovieClip.prototype.repeat = function () {
		this.repeatFlag = true;
	}
	
	/*
	* remove all non-movieclips
	* also remove movieclips that are on last frame but not first frame
	*/
	MovieClip.prototype.cleanup = function (commands, resourceManager) {
		
		var objects = this.root.selectAll('g'),
			j,
			i,
			id,
			shape,
			bitmap,
			text,
			count;
		
		for (j = 0; j < objects.length; j += 1) {
			count = 0;
			id = objects[j].attr('token');
			shape = resourceManager.getShape(id);
			bitmap = resourceManager.getBitmap(id);
			text = resourceManager.getText(id);
			
			for (i = 0; i < commands.length; i += 1) {
				if (commands[i].objectId !== id) {
					count += 1;
				}
			}

			if((shape !== null && shape !== undefined) ||
			   (bitmap !== null && bitmap !== undefined) ||
			   (text !== null && text !== undefined) ||
			   (count == commands.length))
			{
				objects[j].remove();
			}
		}
	}
	
	/**
	* clean up unused defs
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
	}
	
	MovieClip.prototype.clear = function () {
		var items = this.el.selectAll('g'),
			defs,
			defGroups,
			i;
		
		for (i = 0; i < items.length; i += 1) {
			items[i].remove();
		}
		
		defs = this.el.selectAll('defs mask, defs radialGradient, defs linearGradient');
		if (defs) {
			for (i = 0; i < defs.length; i += 1) {
				defs[i].remove();
			}
		}
	}
	
	MovieClip.prototype.runCommands = function (resourceManager) {
		var frame,
			commands,
			c,
			cmdData,
			type,
			command,
			time;
		
		time = Math.round(this.m_timeline.time()) - 1;
		frame = this.m_ctimeline.Frame[time];
		if (!frame) {
			return;
		}
		
		commands = frame.Command;	
		
		if (this.repeatFlag === true) {
			this.cleanup(commands, resourceManager);
			this.repeatFlag = false;
		}
		
		for (c = 0; c < commands.length; c += 1) {
			cmdData = commands[c];
			type = cmdData.cmdType;
			command = undefined;

			switch(type)
			{
				case "Place":
					command = new PlaceObjectCommand(cmdData.charid, cmdData.objectId, cmdData.placeAfter, cmdData.transformMatrix);
				break;

				case "Move":
					command = new MoveObjectCommand(cmdData.objectId, cmdData.transformMatrix);
				break;

				case "Remove":
					command = new RemoveObjectCommand(cmdData.objectId);
				break;
				
				case "UpdateZOrder":
					command = new UpdateObjectCommand(cmdData.objectId , cmdData.placeAfter);
				break;

				case "UpdateVisibility":
					command = new UpdateVisibilityCommand(cmdData.objectId , cmdData.visibility);
				break;

				case "UpdateMask":
					command = new UpdateMaskCommand(cmdData.objectId , cmdData.maskTill);
				break;
			}

			if(command !== undefined) {
				command.execute(this, resourceManager);
			}
		}
		
		this.cleanupUnusedDefs();
	}
	
	MovieClip.prototype.getTimeline = function () {
		return this.m_timeline;
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
				//if already exists do not add
				if (parentMC.select('[token="' + this.m_objectID + '"]')) {
					//run move in case different
					var command = new MoveObjectCommand(this.m_objectID, this.m_transform);
					command.execute(stage, resourceManager);
					//TODO::update z index
					return;
				}
				
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
					
					var tl = movieclip.getTimeline();
					tl.play();
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
			mask,
			masked,
			i,
			def,
			clone;
		
		if(parentMC != undefined)
		{
			baseMask = parentMC.select('[token="' + this.m_objectID + '"]');
			def = baseMask.toDefs();
			
			for (i = parseInt(this.m_maskTill); i > parseInt(this.m_objectID); i -= 1) {
				clone = def.clone(); //issue with reusing def ??
				masked = parentMC.select('[token="' + i + '"]');
				masked.attr({mask: clone});
			}
		}
	}

	return MovieClip;
	
});