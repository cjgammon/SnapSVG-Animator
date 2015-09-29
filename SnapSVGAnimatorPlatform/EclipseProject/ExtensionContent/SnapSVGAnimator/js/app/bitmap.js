	
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

            if (afterMC.isMasked) {  //if masked add outside mask group
                afterMC.el.parent().before(instance.el);
            } else {
                afterMC.el.before(instance.el);
            }

        } else {
            parentEl.add(instance.el); //TODO:: handle after
        }
    };

    this.create();
};
