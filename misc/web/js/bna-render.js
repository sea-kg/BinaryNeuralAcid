class BnaRender {
    
    static defaultBackgroundColor = "rgb(0,0,0)";
    backgroundColor = BnaRender.defaultBackgroundColor;
    
    static defaultBorderColor = "rgb(255, 255, 255)";
    borderColor = BnaRender.defaultBorderColor;

    static defaultInputBackgroundColor = "rgb(255, 255, 255)";
    inputBackgroundColor = BnaRender.defaultInputBackgroundColor;

    static defaultInputTextColor = "rgb(0, 0, 0)";
    inputTextColor = BnaRender.defaultInputTextColor;
    
    static defaultFontSize = 12;
    fontSize = BnaRender.defaultFontSize;

    constructor(elemId) {
        this.elementId = elemId; 
        this.canvas = document.getElementById(this.elementId);
        this.ctx = this.canvas.getContext('2d');
    }

    setData(data) {
        this.inputSize = data.input;
        this.outputSize = data.output;
        this.items = [];
        // copy
        for (var i in data.items) {
            var item = data.items[i];
            this.items.push({
                x: item.x,
                y: item.y,
                o: item.o,
            });
        }
    }

    render() {
        this.ctx.lineWidth = 1;
        this.ctx.fillStyle = this.backgroundColor;
        this.ctx.fillRect(0, 0, this.canvas.width, this.canvas.height);
        this.ctx.strokeStyle = this.borderColor;
        this.ctx.strokeRect(0, 0, this.canvas.width, this.canvas.height);

        // inputs
        for (var i = 0; i < this.inputSize; i++) {
            var p_w = 30;
            var p_h = 20;
            var p_x = 20;
            var p_y = 20 + i * p_h + i * 5;
            
            this.ctx.fillStyle = this.inputBackgroundColor;
            this.ctx.fillRect(p_x, p_y, p_w, p_h);

            this.ctx.font = this.fontSize + "px Arial";
            this.ctx.fillStyle = this.inputTextColor;
            var txt = "in" + i;
            var m = this.ctx.measureText(txt);
            
            this.ctx.fillText(
                txt,
                p_x + p_w/2 - m.width / 2,
                p_y + p_h/2 + this.fontSize/2
            );
        }

    }
};