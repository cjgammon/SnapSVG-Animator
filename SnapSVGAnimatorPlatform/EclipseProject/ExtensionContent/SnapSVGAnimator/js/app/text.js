define(function (require) {
	
    var Text;

    Text = function (parentMC,resourceManager,charId,ObjectId,placeAfter,transform,bounds) {

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
                fontSize,
                fontName,
                fontColor,
                textAlign,
                textAnchor,
                textIndent,
                textBounds;

            textBox = instance.el.g();

            if (bounds) {
                textBounds = bounds.split(',');
            } else {
                textBounds = [0, 0, 200, 100];
            }

            textAlign = data.paras[0].alignment;
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

            text = textBox.text(0, 0, data.txt);
            text.attr({
                'text-anchor': textAnchor,
                'alignment-baseline': 'baseline',
                'font-family': fontName,
                'font-size': fontSize,
                'letter-spacing': letterSpacing,
                'fill': fontColor
            });

            bbox = text.getBBox();
            textX = parseFloat(textBounds[0]) + (parseFloat(textBounds[2]) / 2) - bbox.cx;
            textY = parseFloat(textBounds[1]) + (parseFloat(textBounds[3]) / 2) - bbox.cy;

            textBox.transform('translate(' + textX + ',' + textY + ')');
        };

        this.create();
    };

	return Text;
});
