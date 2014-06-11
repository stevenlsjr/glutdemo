//primitive color
varying vec4 fragColor;
varying vec4 fragCoord;
varying vec2 fragUv;
varying float lightIntensity;

uniform bool do_texture;

uniform float time;
uniform float aspect;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D texture;

const vec4 colorA = vec4(1.0, 0.9, 0.5, 1.0);
const vec4 colorB = vec4(0.8, 0.3, 0.0, 1.0);
const vec4 cA = vec4(0.1, 0.0, 0.2, 1.0);
const vec4 cB = vec4(0.3, 0.0, 0.0, 1.0);
const vec4 cC = vec4(0.9, 1.0, 1.0, 1.0);

vec4 brick()
{
    vec4 color;
    
    vec2 brickSize = vec2(0.3, 0.1);
    vec2 brickPct = vec2(0.95, 0.9);
    vec2 position = fragUv/brickSize;
    if(fract(position.y * 0.5) > 0.5) {
        position.x += 0.5;
    }
    position = fract(position);
    vec2 useBrick = step(position, brickPct);
    color = mix(colorA, colorB, useBrick.x * useBrick.y);
    
    //color *= fragColor;
    
    return color;
}

vec4 tex_color()
{
    vec4 color;

    if (do_texture == true){
        color = fragColor * texture2D(texture, fragUv);
    } else {
        color = fragColor;
    }
    
    return color;
}

vec4 frac_color(int i, float r2)
{
    vec4 color;
    if (r2 < 4.0) {
        color = cA;
    } else {
        color = mix(cB, cC, fract(i * 0.05));
    }

    return color;
}

vec4 mandelbrot(int iter)
{
    vec2 z, c;

    vec2 center = vec2(0.0, 0.0);
    
    float scale = 2;

    float r2 = 0.0;

    c.x = 1.3333 * (fragUv.x - 0.5) * scale - center.x;
    c.y = (fragUv.y - 0.5) * scale - center.y;

    int i;
    z = c;
    for (i=0; i<iter && r2 < 4.0; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        r2 = (x * x + y * y);
        z.x = x;
        z.y = y;
    }

    return frac_color(i, r2);
}

vec4 julia(int iter)
{
    vec2 c = vec2(sin(time), mod(time, 5));

    vec2 z;
    z.x = 3.0 * (fragUv.x - 0.5);
    z.y = 2.0 * (fragUv.y - 0.5);
    float r2 = 0.0;

    int i;
    for(i=0; i<iter && r2 < 4.0; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        r2 = (x * x + y * y);

        z.x = x;
        z.y = y;
    }

    return texture2D(texture,vec2((i == iter ? 0.0 : float(i)) / 100.0, 0));
}

void main(void) {

    gl_FragColor = julia(50);
}

