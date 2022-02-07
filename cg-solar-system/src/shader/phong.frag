#version 330
precision mediump float;

// output from phong.vert --> input from phong.frag
in vec3 v2f_normal;
in vec2 v2f_texcoord;
in vec3 v2f_light;
in vec3 v2f_view;

out vec4 f_color;

uniform sampler2D tex;
uniform bool greyscale;

const float shininess = 8.0;
const vec3  sunlight = vec3(1.0, 0.941, 0.898);

void main()
{
    /**
    *  Implement the Phong shading model (like in the first exercise)
    *  - Use the passed `in` variables to compute the resulting color and store it in `f_color`.
    *  - The texture's color value should be used as material parameter for ambient, diffuse and specular lighting.
    *  - You can copy the function to extract the texture's color from the `color.frag` fragmentshader.
    *  - Scale ambient and specular light component by 0.2 to get a more realistic result
    *  - You do not have to implement shadows.
    *
    *  Hint: Here, functions like reflect, dot, max, min, normalize can be used in the same way as in the raytracer.
     */

    vec3 tex_color = texture(tex, v2f_texcoord.st).rgb;
    float alpha = texture(tex, v2f_texcoord.st).a;

    vec3 color = .2*tex_color;
    color += sunlight*tex_color*max(0,dot(v2f_light,v2f_normal));
    color += .2*sunlight*tex_color*pow(max(0,dot(v2f_view,normalize(reflect(-v2f_light,v2f_normal)))),shininess);

//    float temp = dot(v2f_light,v2f_normal);
//    if(temp > 0) {
//        color += sunlight*tex_color*temp;
//        float temp2 = dot(v2f_view,normalize(reflect(-v2f_light,v2f_normal)));
//        if(temp2 > 0) {
//         color += .2*sunlight*tex_color*pow(temp2,shininess);
//        }
//    }
    
    // convert RGB color to YUV color and use only the luminance
    if (greyscale) color = vec3(0.299*color.r+0.587*color.g+0.114*color.b);

    // add required alpha value
    f_color = vec4(color, alpha);
}
