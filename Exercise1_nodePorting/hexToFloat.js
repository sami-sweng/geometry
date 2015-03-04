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

function csToC(acs){
	var colors=[];
	for (var i=0; i<acs.length; i++){
		var c=acs[i].split(",");
		c[0]=c[0]/255;
		c[1]=c[1]/255;
		c[2]=c[2]/255;
		colors.push(c);
	}
	return colors;
}

function printColors(ac){
	for (var i=0; i<ac.length; i++){
		console.log(colorToString(ac[i]));
	}
}

var s="255,0,0;255,97,0;255,194,0;217,255,0;120,255,0;23,255,0;0,255,74;0,255,171;0,241,255;0,143,255;0,46,255;50,0,255";
var acs=s.split(";");
var ac=csToC(acs);

printColors(ac);

