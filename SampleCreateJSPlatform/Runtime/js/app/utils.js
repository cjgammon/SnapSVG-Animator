define(function (require) {
	
	var Utils = {
		
		CreateShape: function (s,resourceManager,charId,ObjectId,placeAfter,transform)
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
						shape1.attr({fill: 'transparent'});

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
								var p =0;
								var mat = new createjs.Matrix2D(patternArray[p],patternArray[p+1],patternArray[p+1],patternArray[p+3],patternArray[p+4],patternArray[p+5]);
								var image = new Image();
								image.src = resourceManager.m_data.DOMDocument.Shape[k].path[j].image.bitmapPath;						
								shape1.graphics.beginBitmapFill(image,"no-repeat",mat).beginStroke();						
							}
							if(resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient)
							{					
								var stopArray = new Array();
								var offSetArray = new Array();					
								for(var s=0;s<resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.stop.length;s++)
								{								
									stopArray[s] = resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.stop[s].stopColor;
									offSetArray[s] = resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.stop[s].offset/100
								}							
								shape1.graphics.lf(stopArray ,offSetArray,resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.x1,resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.y1,resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.x2,resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.y2);
							}

							if(resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient)
							{						
								var stopsArray = new Array();
								var offSetsArray = new Array();					
								for(var s=0;s<resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient.stop.length;s++)
								{								
									stopsArray[s] = resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient.stop[s].stopColor;
									offSetsArray[s] = resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient.stop[s].offset/100;							
								}
								shape1.graphics.rf(stopsArray,offSetsArray,resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient.cx,resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient.cy,0,resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient.fx,resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient.fy,resourceManager.m_data.DOMDocument.Shape[k].path[j].radialGradient.r);

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
							if(resourceManager.m_data.DOMDocument.Shape[k].path[j].image)
							{ 
								var patternArray = resourceManager.m_data.DOMDocument.Shape[k].path[j].image.patternTransform.split(",");
								var p =0;
								var mat = new createjs.Matrix2D(patternArray[p],patternArray[p+1],patternArray[p+1],patternArray[p+3],patternArray[p+4],patternArray[p+5]);
								var image = new Image();
								image.src = resourceManager.m_data.DOMDocument.Shape[k].path[j].image.bitmapPath;
								shape1.graphics.beginBitmapStroke(image,"no-repeat").beginStroke().setStrokeStyle(data.DOMDocument.Shape[k].path[j].strokeWidth,resourceManager.m_data.DOMDocument.Shape[k].path[j].strokeLinecap,resourceManager.m_data.DOMDocument.Shape[k].path[j].strokeLinejoin);
							}						
							if(resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient)
							{
								var stopArray = new Array();
								var offSetArray = new Array();
								for(var s=0;s<resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.stop.length;s++)
								{
									stopArray[s] = resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.stop[s].stopColor;
									offSetArray[s] = resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.stop[s].offset/100;							
								}
								shape1.graphics.ls(stopArray,offSetArray,resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.x1,resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.y1,resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.x2,resourceManager.m_data.DOMDocument.Shape[k].path[j].linearGradient.y2).setStrokeStyle(resourceManager.m_data.DOMDocument.Shape[k].path[j].strokeWidth,resourceManager.m_data.DOMDocument.Shape[k].path[j].strokeLinecap,resourceManager.m_data.DOMDocument.Shape[k].path[j].strokeLinejoin);;
							}
						}
						var path = resourceManager.m_data.DOMDocument.Shape[k].path[j].d;
						shape1.attr({d: path});
						pathContainer.add(shape1);
					}
				}
			}

			var transformArray = transform.split(",");
			var scaleX,scaleY,rotation,skewX,skewY;
			var TransformMat = new Snap.Matrix(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5]);

			pathContainer.transform(TransformMat.toTransformString());
			
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
		}

		/*
		function CreateBitmap(parentMC,resourceManager,charId,ObjectId,placeAfter,transform)
		{
		for(var b =0;b<resourceManager.m_data.DOMDocument.Bitmaps.length;b++)
			{
				if(resourceManager.m_data.DOMDocument.Bitmaps[b].charid == charId)
				{
				var path = resourceManager.m_data.DOMDocument.Bitmaps[b].bitmapPath;
				var bitmap = new createjs.Bitmap(path);
				bitmap.id = parseInt(ObjectId);
				}

			}
			var transformArray = transform.split(",");
			var scaleX,scaleY,rotation,skewX,skewY;
			var TransformMat = new createjs.Matrix2D(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5])
			scaleX = Math.sqrt((transformArray[0]*transformArray[0])+ (transformArray[1]*transformArray[1]));
			scaleY = Math.sqrt((transformArray[2]*transformArray[2]) + (transformArray[3]*transformArray[3]));
			skewX = Math.atan2(-(transformArray[2]), transformArray[3]);
			skewY = Math.atan2(transformArray[1], transformArray[0]);			
			skewX = skewX * (180*7/22);
			skewY=skewY *(180*7/22);
			bitmap.setTransform(transformArray[4],transformArray[5],scaleX,scaleY,0,skewX,skewY);

			if(parentMC != undefined)
			{				
				if(placeAfter != 0)
				{				
					for(var index =0 ; index<parentMC.children.length; index++)
					{
						if(parentMC.children[index].id == parseInt(placeAfter))
						{
							parentMC.addChildAt(bitmap , index);
							index ++;							
						}					
					}
				}
				else
				{
					parentMC.addChild(bitmap);
				}				
				while(parentMC.mode != undefined)
				{
					parentMC.getStage();	 
					parentMC = parentMC.parent;
				}				 

				parentMC.update();
			}
			else
			{
				stage.addChildAt(bitmap);	
				stage.update();				
			}
		}
		*/

		/*
		function CreateText(parentMC,resourceManager,charId,ObjectId,placeAfter,transform)
		{
		for(var b =0;b<resourceManager.m_data.DOMDocument.Text.length;b++)
			{
				if(resourceManager.m_data.DOMDocument.Text[b].charid == charId)
				{
				var displayString = resourceManager.m_data.DOMDocument.Text[b].displayText;
				var txt = displayString.replace(/\\r/g,"\r");

				var font = resourceManager.m_data.DOMDocument.Text[b].font;
				var fontColor = resourceManager.m_data.DOMDocument.Text[b].color;
				var textOutput = new createjs.Text(txt,font,fontColor);
				textOutput.id = parseInt(ObjectId);
				}

			}
			var transformArray = transform.split(",");
			var scaleX,scaleY,rotation,skewX,skewY;
			var TransformMat = new createjs.Matrix2D(transformArray[0],transformArray[1],transformArray[2],transformArray[3],transformArray[4],transformArray[5])
			scaleX = Math.sqrt((transformArray[0]*transformArray[0])+ (transformArray[1]*transformArray[1]));
			scaleY = Math.sqrt((transformArray[2]*transformArray[2]) + (transformArray[3]*transformArray[3]));
			skewX = Math.atan2(-(transformArray[2]), transformArray[3]);
			skewY = Math.atan2(transformArray[1], transformArray[0]);			
			skewX = skewX * (180*7/22);
			skewY=skewY *(180*7/22);
			textOutput.setTransform(transformArray[4],transformArray[5],scaleX,scaleY,0,skewX,skewY);

			if(parentMC != undefined)
			{				
				if(placeAfter != 0)
				{				
					for(var index =0 ; index<parentMC.children.length; index++)
					{
						if(parentMC.children[index].id == parseInt(placeAfter))
						{
							parentMC.addChildAt(textOutput , index);
							index ++;							
						}					
					}
				}
				else
				{
					parentMC.addChild(textOutput);
				}				
				while(parentMC.mode != undefined)
				{
					parentMC.getStage();	 
					parentMC = parentMC.parent;
				}				 

				parentMC.update();
			}
			else
			{
				stage.addChildAt(textOutput);	
				stage.update();				
			}
		}
		*/
		
	}
	
	return Utils;
});