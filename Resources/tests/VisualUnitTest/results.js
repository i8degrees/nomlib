/*	Derives from the Visual Unit Testing framework for Ogre3D by
		Riley Adams <praetor57@gmail.com>.

		http://www.ogre3d.org/tikiwiki/Visual+Unit+Testing+Framework
*/

// save some typing
function $(id)
{
	return document.getElementById(id);
}

function getChildElement(index)
{
	var ind = 0;
	for(var i = 0; i < this.childNodes.length; ++i)
	{
		if(this.childNodes[i].nodeType == 1)
		{
			if(index == ind)
			{
				return this.childNodes[i];
			}
			++ind;
		}
	}
	return null;
}

function getNumChildElements()
{
	var num = 0;
	for(var i = 0; i < this.childNodes.length; ++i)
	{
		if(this.childNodes[i].nodeType == 1)
			++num;
	}
	return num;
}

Element.prototype.getNumChildElements = getNumChildElements;
Element.prototype.getChildElement = getChildElement;

var entries;
var imgx = 0;
var imgy = 0;

window.onload = function()
{
	entries = [];

	// get res from the stat table (since chrome is iffy about reading img dimensions)
	var res = document.body.getElementsByTagName("table")[0].rows[2].
		getChildElement(1).innerHTML;
	imgx = parseInt(res);
	imgy = parseInt(res.split("x ")[1]);

	// insert buttons
	for(var i = 2, n = document.body.getNumChildElements(); i < n; ++i)
	{
		var btn = document.createElement("button");
		btn.innerHTML = "Toggle Diff";
		btn.parentEntry = document.body.getChildElement(i);
		btn.onclick = toggleDiff;
		document.body.getChildElement(i).getChildElement(1).insertBefore(btn,
			document.body.getChildElement(i).getChildElement(1).getChildElement(2));
		entries.push(document.body.getChildElement(i));
		document.body.getChildElement(i).diffOn = false;

		var btn2 = document.createElement("button");
		btn2.innerHTML = "Swap Images";
		btn2.parentEntry = document.body.getChildElement(i);
		btn2.onclick = swapImgs;
		document.body.getChildElement(i).getChildElement(1).insertBefore(btn2,
			document.body.getChildElement(i).getChildElement(1).getChildElement(2));

		document.body.getChildElement(i).hasDiff = false;
	}
};

function toggleDiff()
{
	// necessary for local image access in FF
	//netscape.security.PrivilegeManager.enablePrivilege("UniversalBrowserRead");

	var entry = this.parentEntry;
	var carea = entry.getChildElement(1);
	var col1 = carea.getElementsByTagName("div")[0];
	var col2 = carea.getElementsByTagName("div")[1];
	var img1 = col1.getElementsByTagName("img")[0];
	var img2 = col2.getElementsByTagName("img")[0];
	var can = null;

	if(entry.hasDiff)
	{
		can = carea.getChildElement(2);

		if(entry.diffOn)
		{
			can.style.display = "none";
			col2.style.display = "block";
		}
		else
		{
			can.style.display = "block";
			col2.style.display = "none";
		}

		entry.diffOn = !entry.diffOn;
	}
	else
	{
		// hide original images
		col1.style.display = "none";
		col2.style.display = "none";

		// make a column for this to live in
		var column = document.createElement("div");
		column.className = "img_column";
		// and a header
		var header = document.createElement("h3");
		header.innerHTML = "Diff:";
		column.appendChild(header);

		// make a couple canvases so we can read image data
		var canvas1 = document.createElement("canvas");
		canvas1.width = imgx;
		canvas1.height = imgy;
		var canvas2 = document.createElement("canvas");
		canvas2.width = imgx;
		canvas2.height = imgy;

		// get ImageData from contexts
		var context = canvas1.getContext("2d");
		var context2 = canvas2.getContext("2d");
		context.drawImage(img1, 0, 0);
		context2.drawImage(img2, 0, 0);
		var data = context.getImageData(0, 0, imgx, imgy);
		var data2 = context2.getImageData(0, 0, imgx, imgy);
		var px1 = data.data;
		var px2 = data2.data;

		// do the diffing
		for(var i = 0, n = px1.length; i < n; i+=4)
		{
			px1[i] = px1[i] > px2[i] ? px1[i] - px2[i] : px2[i] - px1[i];
			px1[i+1] = px1[i+1] > px2[i+1] ? px1[i+1] - px2[i+1] : px2[i+1] - px1[i+1];
			px1[i+2] = px1[i+2] > px2[i+2] ? px1[i+2] - px2[i+2] : px2[i+2] - px1[i+2];
		}

		// save to one of the canvases
		context.putImageData(data, 0, 0);

		// and insert the canvas
		canvas1.style.width = "480px";
		column.appendChild(canvas1);
		carea.insertBefore(column, carea.getChildElement(2));

		entry.hasDiff = true;
		entry.diffOn = true;
		col1.style.display = "block";
	}
}

function swapImgs()
{
	var entry = this.parentEntry;
	var carea = entry.getChildElement(1);
	var divs = carea.getElementsByTagName("div");
	for(var i = 0; i < divs.length; ++i)
	{
		if(divs[i].style.cssFloat == "right")
			divs[i].style.cssFloat = "left";
		else
			divs[i].style.cssFloat = "right";
	}
}

// function on_click(e)
// {
// 	var evt = e ? e:window.event;
// 	var mouse_x = 0, mouse_y = 0;

// 	if( evt.pageX || evt.pageY )
// 	{
// 		mouse_x = evt.pageX;
// 		mouse_y = evt.pageY;
// 	}
// 		// var header = document.createElement("h3");
// 		// header.innerHTML = "Diff:";
// 		// column.appendChild(header);
// 	var testme = 0;
// 	// var b = document.getElementsByTagName("body");
// 	var b = document.getElementById(33);

// 	// testme[0] = "mouse_x";
// 	b = mouse_x;
// 	console.log("x: ",mouse_x);
// 	console.log("y: ",mouse_y);
// 	console.log("x+x: ",mouse_x+mouse_x);
// 	// testme.appendChild(testme);
// 	// alert( mouse_x + ' ' + mouse_y )

// 	return false;
// }

// $(canvas).click(function(e) {
// // function on_click(arg)
//     var canvasOffset = $(canvas).offset();
//     // var canvasOffset = arg.offset();
//     var canvasX = Math.floor(e.pageX-canvasOffset.left);
//     var canvasY = Math.floor(e.pageY-canvasOffset.top);

//     var imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);
//     var pixels = imageData.data;
//     var pixelRedIndex = ((canvasY - 1) * (imageData.width * 4)) + ((canvasX - 1) * 4);
//     var pixelcolor = "rgba("+pixels[pixelRedIndex]+", "+pixels[pixelRedIndex+1]+", "+pixels[pixelRedIndex+2]+", "+pixels[pixelRedIndex+3]+")";

//     $("body").css("backgroundColor", pixelcolor);
// });
