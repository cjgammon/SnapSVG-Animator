
var GarbagePool = function () {
    this.EMPTY_POOL = [];      //if empty remove
    this.REF_POOL = [];        //if no reference remove
};

GarbagePool.prototype.addEmpty = function (el) {
    this.EMPTY_POOL.push(el);
};

/**
 * manages adding of references
 *
 */
GarbagePool.prototype.addRef = function (el, refs) {
    var i,
        j;

    for (i = 0; i < this.REF_POOL.length; i += 1) {
        if (this.REF_POOL[i].el.id == el.id) {
            for (j = 0; j < refs.length; j += 1) {
                this.REF_POOL[i].refs.push(refs[j]);   
            }
            return;
        }
    }

    this.REF_POOL.push({el: el, refs: refs});
};

GarbagePool.prototype.purge = function () {
    this.purgeEmptyPool();
    this.purgeRefPool();
};

/**
 * check if empty and remove
 *
 */
GarbagePool.prototype.purgeEmptyPool = function () {
    var i,
        el;

    for (i = this.EMPTY_POOL.length - 1; i > -1; i -= 1) {
        el = this.EMPTY_POOL[i];
        if (el.children().length === 0) {
            el.remove();
            this.EMPTY_POOL.splice(i, 1);
        }
    }
};

/**
 * check if all references are removed then remove
 *
 */
GarbagePool.prototype.purgeRefPool = function () {
    var i,
        j,
        k,
        item;

    for (i = this.REF_POOL.length - 1; i > -1; i -= 1) {

        item = this.REF_POOL[i];
        k = 0;

        for (j = 0; j < item.refs.length; j += 1) {

            if (item.refs[j].removed) {
                k += 1;
            }

            if (k == item.refs.length) {
                item.el.remove();
                this.REF_POOL.splice(i, 1);
            }
        }
    }
};

var GP = new GarbagePool();
