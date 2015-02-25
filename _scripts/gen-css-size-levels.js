var start = 320;
var end = 2000+1;
var base = 900; // at zoom:1
var step = 1;
var width;

for (width = start; width != end; width += step) {
  console.log('@media screen and (min-width: '+width+'px) { '+
              'body { zoom: '+(width/base).toFixed(3)+'; } '+
              '}');
}
