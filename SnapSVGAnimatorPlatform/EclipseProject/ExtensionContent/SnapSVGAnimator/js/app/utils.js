define(function (require) {
	
    var Utils,
        DisplayObject;

	function hexToRgb(hex) {
    	var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    	return result ? {
        	r: parseInt(result[1], 16),
        	g: parseInt(result[2], 16),
        	b: parseInt(result[3], 16)
    	} : null;
	}

    DisplayObject = function (parentMC) {
        this.el = parentMC.g();
    };

	Utils = {
		
		CreateShape: function (root, s, resourceManager,charId,ObjectId,placeAfter,transform)
		{
			var pathContainer = s.g();
			pathContainer.attr({token: parseInt(ObjectId)});

			for(var k =0;k<resourceManager.m_data.DOMDocument.Shape.length;k++)
			{
				if(resourceManager.m_data.DOMDocument.Shape[k].charid == charId)
				{
					for(var j=0;j<resourceManager.m_data.DOMDocument.Shape[k].path.length;j++)
					{
						var clr,clrOpacity;
						var shape1 = s.path();
						var path = resourceManager.m_data.DOMDocument.Shape[k].path[j].d;
						shape1.attr({fill: 'transparent'});
						shape1.attr({d: path});
						
						if(resourceManager.m_data.DOMDocument.Shape[k].path[j].pathType == "Fill")
						{
							if(resourceManager.m_data.DOMDocument.Shape[k].path[j].color)
							{
								clr = resourceManager.m_data.DOMDocument.Shape[k].path[j].color;
		                        var r = parseInt(clr.substring(1, 3), 16);
		                        var g = parseInt(clr.substring(3, 5), 16);
		                        var b = parseInt(clr.substring(5, 7), 16);
		                        var colStr = 'rgba(' + r + ',' + g + ',' + b + ',' + resourceManager.m_data.DOMDocument.Shape[k].path[j].colorOpacity + ')';
								shape1.attr({fill: colStr});
							}
							if(resourceManager.m_data.DOMDocument.Shape[k].path[j].image)
							{ 
								var patternArray = resourceManager.m_data.DOMDocument.Shape[k].path[j].image.patternTransform.split(",");						
								var p = 0;
								var mat = new Snap.Matrix(patternArray[p],patternArray[p+1],patternArray[p+1],patternArray[p+3],patternArray[p+4],patternArray[p+5]);
								var src = resourceManager.m_data.DOMDocument.Shape[k].path[j].image.bitmapPath;
								
								function newPattern() {
									var image = s.image(src);
									var pattern = image.pattern(0, 0, resourceManager.m_data.DOMDocument.Shape[k].path[j].image.width, resourceManager.m_data.DOMDocument.Shape[k].path[j].image.height);
									pattern.attr({x: mat.e, y: mat.f});
									shape1.attr({fill: pattern});
								}
								
								var exists = root.select('defs pattern image');
								
								if (exists) {
									if (exists.attr('href') == src) {	//check if image href matches as well as other props
										shape1.attr({fill: exists.parent()});
									} else {
										newPattern();
									}
								} else {
									newPattern();
								}

							}
							if(resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient)
							{							
								var _x = parseFloat(resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.x1);
								var _y = parseFloat(resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.y1);
								var _x2 = parseFloat(resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.x2);
								var _y2 = parseFloat(resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.y2);
								var gradientString = "L(";
								gradientString += _x + ", ";
								gradientString += _y + ", ";
								gradientString += _x2 + ", ";
								gradientString += _y2 + ")";
								
								for(var i=0; i<resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.stop.length; i++)
								{	
									var rgb = hexToRgb(resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.stop[i].stopColor);
									gradientString += 'rgba(' + rgb.r + ',' + rgb.g + ',' + rgb.b + ',' + resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.stop[i].stopOpacity + ')';		
									gradientString += ":";
									gradientString += resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.stop[i].offset;
									if (i !== resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.stop.length-1) {
										gradientString += "-";
									}		
								}
								var _g = root.gradient(gradientString);
								shape1.attr({fill: _g});
							}

							if(resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient)
							{	
								var _x = (shape1.getBBox().x + shape1.getBBox().width / 2) + resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient.cx / 10;
								var _y = (shape1.getBBox().y + shape1.getBBox().height / 2) + resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient.cy / 10;
								var _fx = (shape1.getBBox().x + shape1.getBBox().width / 2) + parseFloat(resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient.fx);
								var _fy = (shape1.getBBox().y + shape1.getBBox().height / 2) + parseFloat(resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient.fy);
								
								var gradientString = "R("; 
								gradientString += _x + ", ";
								gradientString += _y + ", ";
								gradientString += resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient.r + ", ";
								gradientString += _fx + ", ";
								gradientString += _fy + ")";
								
								for(var i=0; i<resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient.stop.length; i++)
								{	
									var rgb = hexToRgb(resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient.stop[i].stopColor);
									gradientString += 'rgba(' + rgb.r + ',' + rgb.g + ',' + rgb.b + ',' + resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient.stop[i].stopOpacity + ')';
									gradientString += ":";
									gradientString += resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient.stop[i].offset;
									if (i !== resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient.stop.length-1) {
										gradientString += "-";
									}		
								}
								
								var _g = root.gradient(gradientString);
								shape1.attr({fill: _g});
							}
						}
						else if(resourceManager.m_data.DOMDocument.Shape[k].path[j].pathType == "Stroke")
						{
							if(resourceManager.m_data.DOMDocument.Shape[k].path[j].color)
							{
								clr = resourceManager.m_data.DOMDocument.Shape[k].path[j].color;

		                        var r = parseInt(clr.substring(1, 3), 16);
		                        var g = parseInt(clr.substring(3, 5), 16);
		                        var b = parseInt(clr.substring(5, 7), 16);

		                        var colStr = 'rgba(' + r + ',' + g + ',' + b + ',' + resourceManager.m_data.DOMDocument.Shape[k].path[j].colorOpacity + ')';

								shape1.attr({stroke: colStr, strokeWidth: resourceManager.m_data.DOMDocument.Shape[k].path[j].strokeWidth});
							}
							/*
							if(resourceManager.m_data.DOMDocument.Shape[k].path[j].image)
							{ 
								var patternArray = resourceManager.m_data.DOMDocument.Shape[k].path[j].image.patternTransform.split(",");
								var p = 0;
								var mat = new createjs.Matrix2D(patternArray[p],patternArray[p+1],patternArray[p+1],patternArray[p+3],patternArray[p+4],patternArray[p+5]);
								var image = new Image();
								image.src = resourceManager.m_data.DOMDocument.Shape[k].path[j].image.bitmapPath;
								shape1.graphics.beginBitmapStroke(image,"no-repeat").beginStroke().setStrokeStyle(data.DOMDocument.Shape[k].path[j].strokeWidth,resourceManager.m_data.DOMDocument.Shape[k].path[j].strokeLinecap,resourceManager.m_data.DOMDocument.Shape[k].path[j].strokeLinejoin);
							}
							*/
						}
						//var path = resourceManager.m_data.DOMDocument.Shape[k].path[j].d;
						//shape1.attr({d: path});
						pathContainer.add(shape1);
					}
				}
			}

			var transformArray = transform.split(",");
			var TransformMat = new Snap.Matrix(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5]);
			pathContainer.transform(TransformMat);
			
			if(s != undefined)
			{				
				if(placeAfter != 0)
				{
					children = s.selectAll('[token="' + placeAfter + '"]');
					for (i = 0; i < children.length; i += 1) {
						if (children[i].parent().id == s.id) { //ensure child of current movie clip
							children[i].before(pathContainer);
							break;
						}
					} 
				}
				else
				{
					s.add(pathContainer);	
				}
				
			}
		},

		CreateBitmap: function(root, s,resourceManager,charId,ObjectId,placeAfter,transform)
		{
			var bmContainer = s.g();
			bmContainer.attr({token: parseInt(ObjectId)});

			for(var b =0;b<resourceManager.m_data.DOMDocument.Bitmaps.length;b++)
			{
				if(resourceManager.m_data.DOMDocument.Bitmaps[b].charid == charId)
				{
					var path = resourceManager.m_data.DOMDocument.Bitmaps[b].bitmapPath;
					var bitmap = s.image(path);
					bmContainer.add(bitmap);
				}
			}

			var transformArray = transform.split(",");
			var TransformMat = new Snap.Matrix(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5]);
			bmContainer.transform(TransformMat);

			if(s != undefined)
			{				
				if(placeAfter != 0)
				{				
					children = s.selectAll('[token="' + placeAfter + '"]');
					for (i = 0; i < children.length; i += 1) {
						if (children[i].parent().id == s.id) { //ensure child of current movie clip
							children[i].before(bmContainer);
							break;
						}
					}	
				}
				else
				{
					s.add(bmContainer);
				}		
				/*		
				while(parentMC.mode != undefined)
				{
					parentMC.getStage();	 
					parentMC = parentMC.parent;
				}				 

				parentMC.update();
				*/
			}
			/*
			else
			{
				s.add(bmContainer);	
				//stage.update();				
			}
			*/
		},

		CreateText: function (root, s,resourceManager,charId,ObjectId,placeAfter,transform)
		{
			var textContainer = s.g();
			textContainer.attr({token: parseInt(ObjectId)});
			
			for(var b =0;b<resourceManager.m_data.DOMDocument.Text.length;b++)
			{
				if(resourceManager.m_data.DOMDocument.Text[b].charid == charId)
				{
					var displayString = resourceManager.m_data.DOMDocument.Text[b].txt;
					var txt = displayString.replace(/\\r/g,"\r");
					var fontFamily = resourceManager.m_data.DOMDocument.Text[b].paras[0].textRun[0].style.fontName;
					var fontStyle = resourceManager.m_data.DOMDocument.Text[b].paras[0].textRun[0].style.fontStyle;
					var fontSize = resourceManager.m_data.DOMDocument.Text[b].paras[0].textRun[0].style.fontSize;
					var fontColor = resourceManager.m_data.DOMDocument.Text[b].paras[0].textRun[0].style.fontColor;
					var textOutput = s.text(0, 0, txt);
					textOutput.attr({
						fill: fontColor, 
						fontFamily: fontFamily, 
						fontSize: fontSize, 
						fontStyle: fontStyle
					});
					
					textContainer.add(textOutput);
				}
			}
			
			var transformArray = transform.split(",");
			var TransformMat = new Snap.Matrix(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5]);
			textContainer.transform(TransformMat);
				
			if(s != undefined)
			{				
				if(placeAfter != 0)
				{	
					children = s.selectAll('[token="' + placeAfter + '"]');
					for (i = 0; i < children.length; i += 1) {
						if (children[i].parent().id == s.id) { //ensure child of current movie clip
							children[i].before(textContainer);
							break;
						}
					}	
				}
				else
				{
					s.add(textContainer);
				}	
				/*			
				while(s.mode != undefined)
				{
					parentMC.getStage();	 
					parentMC = parentMC.parent;
				}				 

				parentMC.update();
				*/
			}
			/*
			else
			{
				stage.addChildAt(textOutput);	
				stage.update();				
			}
			*/
		}
		
	}
	
	return Utils;
});
