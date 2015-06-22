define(function (require) {
	
    var Shape;

	function hexToRgb(hex) {
    	var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
    	return result ? {
        	r: parseInt(result[1], 16),
        	g: parseInt(result[2], 16),
        	b: parseInt(result[3], 16)
    	} : null;
	}

    function newPattern(src, resourceImg) {
        var image,
            pattern;

        image = s.image(src);
        pattern = image.pattern(0, 0, resourceImg.width, resourceImg.height);
        pattern.attr({x: mat.e, y: mat.f});

        return pattern;
    }

    Shape = function (parentMC,resourceManager,charId,ObjectId,placeAfter,transform) {
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
                    fillImage = newPattern(src, resourceImg);
                }
            } else {
                fillImage = newPattern(src, resourceImg);
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

        this.create();
    };

    /*
	Shape = {
		
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
							//if(resourceManager.m_data.DOMDocument.Shape[k].path[j].image)
							//{ 
							//	var patternArray = resourceManager.m_data.DOMDocument.Shape[k].path[j].image.patternTransform.split(",");
							//	var p = 0;
							//	var mat = new createjs.Matrix2D(patternArray[p],patternArray[p+1],patternArray[p+1],patternArray[p+3],patternArray[p+4],patternArray[p+5]);
							//	var image = new Image();
							//	image.src = resourceManager.m_data.DOMDocument.Shape[k].path[j].image.bitmapPath;
							//	shape1.graphics.beginBitmapStroke(image,"no-repeat").beginStroke().setStrokeStyle(data.DOMDocument.Shape[k].path[j].strokeWidth,resourceManager.m_data.DOMDocument.Shape[k].path[j].strokeLinecap,resourceManager.m_data.DOMDocument.Shape[k].path[j].strokeLinejoin);
							//}
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
		}
	};
	*/
	return Shape;
});
