function remapValues(values,targetMin,targetMax){
	var remappedValues = [];
        
	// Get sourceDomain min and max
	var srcMin = Math.min.apply(null, values);
	var srcMax = Math.max.apply(null, values);
	
	for (var i=0; i<values.length; i++){
		var v=values[i];
		rv = ((v-srcMin)/(srcMax-srcMin))*(targetMax-targetMin)+targetMin
		remappedValues.push(rv)
	}
            
    return remappedValues
}

function mapValueListAsColors(values,min,max){
	var colors = [];
	var remappedValues = remapValues(values,min,max);
	for (var i=0; i<remappedValues.length; i++){
		var v=remappedValues[i];
        var c = hslToRgb(v,1.0,0.5);
        colors.push(c)
    }
    return colors;
}

function hslToRgb(h, s, l){
    var r, g, b;

    if(s == 0){
        r = g = b = l; // achromatic
    }else{
        var hue2rgb = function hue2rgb(p, q, t){
            if(t < 0) t += 1;
            if(t > 1) t -= 1;
            if(t < 1/6) return p + (q - p) * 6 * t;
            if(t < 1/2) return q;
            if(t < 2/3) return p + (q - p) * (2/3 - t) * 6;
            return p;
        }

        var q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        var p = 2 * l - q;
        r = hue2rgb(p, q, h + 1/3);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1/3);
    }

    return [r,g,b];
}

function colorToString(c){
	return "Color("+
		floatToString(c[0])+","+
		floatToString(c[1])+","+
		floatToString(c[2])+"),";
}

function floatToString(f){
	if (parseInt(f)==f){
		return f+".0f";
	}
	else{
		return f+"f";
	}
}

function printColors(ac){
	for (var i=0; i<ac.length; i++){
		console.log(colorToString(ac[i]));
	}
}

function main(){
	var valences=[3,4,5,6,7,8,9,10,11];
	var colors=mapValueListAsColors(valences,0.0,0.7);
	printColors(colors);
}

main();