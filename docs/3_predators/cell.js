class Cell {
    ['vx0'] = 0x0;
    ['vy0'] = 0x0;
    ['vx1'] = 0x0;
    ['vy1'] = 0x0;
    ['vx2'] = 0x0;
    ['vy2'] = 0x0;
    ['vx3'] = 0x0;
    ['vy3'] = 0x0;
    ['pressure0'] = 0x0;
    ['pressure1'] = 0x0;
    ['pressure2'] = 0x0;
    ['pressure3'] = 0x0;
    ['c7'] = null;
    ['c8'] = null;
    ['c9'] = null;
    ['c4'] = null;
    ['c6'] = null;
    ['c1'] = null;
    ['c2'] = null;
    ['c3'] = null;
    ['updatePressure']() {
        var a = (this['c7']['vx0'] + this['c1']['vx0'] - this['c9']['vx0'] - this['c3']['vx0']) * 0.5 + this['c4']['vx0'] - this['c6']['vx0'];
        var b = (this['c7']['vy0'] + this['c9']['vy0'] - this['c1']['vy0'] - this['c3']['vy0']) * 0.5 + this['c8']['vy0'] - this['c2']['vy0'];
        this['pressure0'] = (a + b) * 0.25;
        var c = (this['c7']['vx1'] + this['c1']['vx1'] - this['c9']['vx1'] - this['c3']['vx1']) * 0.5 + this['c4']['vx1'] - this['c6']['vx1'];
        var d = (this['c7']['vy1'] + this['c9']['vy1'] - this['c1']['vy1'] - this['c3']['vy1']) * 0.5 + this['c8']['vy1'] - this['c2']['vy1'];
        this['pressure1'] = (c + d) * 0.25;
        var e = (this['c7']['vx2'] + this['c1']['vx2'] - this['c9']['vx2'] - this['c3']['vx2']) * 0.5 + this['c4']['vx2'] - this['c6']['vx2'];
        var f = (this['c7']['vy2'] + this['c9']['vy2'] - this['c1']['vy2'] - this['c3']['vy2']) * 0.5 + this['c8']['vy2'] - this['c2']['vy2'];
        this['pressure2'] = (e + f) * 0.25;
        var g = (this['c7']['vx3'] + this['c1']['vx3'] - this['c9']['vx3'] - this['c3']['vx3']) * 0.5 + this['c4']['vx3'] - this['c6']['vx3'];
        var h = (this['c7']['vy3'] + this['c9']['vy3'] - this['c1']['vy3'] - this['c3']['vy3']) * 0.5 + this['c8']['vy3'] - this['c2']['vy3'];
        this['pressure3'] = (g + h) * 0.25;
    }
    ['adaptVelocity']() {
        this['vx0'] += ((this['c7']['pressure0'] + this['c1']['pressure0'] - this['c9']['pressure0'] - this['c3']['pressure0']) * 0.5 + this['c4']['pressure0'] - this['c6']['pressure0']) * 0.25;
        this['vy0'] += ((this['c7']['pressure0'] + this['c9']['pressure0'] - this['c1']['pressure0'] - this['c3']['pressure0']) * 0.5 + this['c8']['pressure0'] - this['c2']['pressure0']) * 0.25;
        this['vx0'] *= CELL_RESISTANCE;
        this['vy0'] *= CELL_RESISTANCE;
        this['vx1'] += ((this['c7']['pressure1'] + this['c1']['pressure1'] - this['c9']['pressure1'] - this['c3']['pressure1']) * 0.5 + this['c4']['pressure1'] - this['c6']['pressure1']) * 0.25;
        this['vy1'] += ((this['c7']['pressure1'] + this['c9']['pressure1'] - this['c1']['pressure1'] - this['c3']['pressure1']) * 0.5 + this['c8']['pressure1'] - this['c2']['pressure1']) * 0.25;
        this['vx1'] *= CELL_RESISTANCE;
        this['vy1'] *= CELL_RESISTANCE;
        this['vx2'] += ((this['c7']['pressure2'] + this['c1']['pressure2'] - this['c9']['pressure2'] - this['c3']['pressure2']) * 0.5 + this['c4']['pressure2'] - this['c6']['pressure2']) * 0.25;
        this['vy2'] += ((this['c7']['pressure2'] + this['c9']['pressure2'] - this['c1']['pressure2'] - this['c3']['pressure2']) * 0.5 + this['c8']['pressure2'] - this['c2']['pressure2']) * 0.25;
        this['vx2'] *= CELL_RESISTANCE;
        this['vy2'] *= CELL_RESISTANCE;
        this['vx3'] += ((this['c7']['pressure3'] + this['c1']['pressure3'] - this['c9']['pressure3'] - this['c3']['pressure3']) * 0.5 + this['c4']['pressure3'] - this['c6']['pressure3']) * 0.25;
        this['vy3'] += ((this['c7']['pressure3'] + this['c9']['pressure3'] - this['c1']['pressure3'] - this['c3']['pressure3']) * 0.5 + this['c8']['pressure3'] - this['c2']['pressure3']) * 0.25;
        this['vx3'] *= CELL_RESISTANCE;
        this['vy3'] *= CELL_RESISTANCE;
        if (this['vx0'] > MAX_CELL_VELOCITY)
            this['vx0'] = MAX_CELL_VELOCITY;
        else if (this['vx0'] < -MAX_CELL_VELOCITY)
            this['vx0'] = -MAX_CELL_VELOCITY;
        if (this['vy0'] > MAX_CELL_VELOCITY)
            this['vy0'] = MAX_CELL_VELOCITY;
        else if (this['vy0'] < -MAX_CELL_VELOCITY)
            this['vy0'] = -MAX_CELL_VELOCITY;
        if (this['vx1'] > MAX_CELL_VELOCITY)
            this['vx1'] = MAX_CELL_VELOCITY;
        else if (this['vx1'] < -MAX_CELL_VELOCITY)
            this['vx1'] = -MAX_CELL_VELOCITY;
        if (this['vy1'] > MAX_CELL_VELOCITY)
            this['vy1'] = MAX_CELL_VELOCITY;
        else if (this['vy1'] < -MAX_CELL_VELOCITY)
            this['vy1'] = -MAX_CELL_VELOCITY;
        if (this['vx2'] > MAX_CELL_VELOCITY)
            this['vx2'] = MAX_CELL_VELOCITY;
        else if (this['vx2'] < -MAX_CELL_VELOCITY)
            this['vx2'] = -MAX_CELL_VELOCITY;
        if (this['vy2'] > MAX_CELL_VELOCITY)
            this['vy2'] = MAX_CELL_VELOCITY;
        else if (this['vy2'] < -MAX_CELL_VELOCITY)
            this['vy2'] = -MAX_CELL_VELOCITY;
        if (this['vx3'] > MAX_CELL_VELOCITY)
            this['vx3'] = MAX_CELL_VELOCITY;
        else if (this['vx3'] < -MAX_CELL_VELOCITY)
            this['vx3'] = -MAX_CELL_VELOCITY;
        if (this['vy3'] > MAX_CELL_VELOCITY)
            this['vy3'] = MAX_CELL_VELOCITY;
        else if (this['vy3'] < -MAX_CELL_VELOCITY)
            this['vy3'] = -MAX_CELL_VELOCITY;
    }
}