#define PI 3.1415926
const int KEY_r     = 82;
const int KEY_b     = 66;
const int KEY_t     = 84;

float intersectSphere(in vec3 ro, in vec3 rd, in vec4 pr)
{
    vec3 v_surface_camera = ro - pr.xyz;
    float sphere_rad = pr.w;
    
    float b = 2.0 * dot(v_surface_camera, rd);
    float c = dot(v_surface_camera, v_surface_camera) - sphere_rad * sphere_rad;
    float h = b*b - 4.0 * c;

    if(h < 0.0) 
    {
        return -1.0;
    }
    return (-b - sqrt(h)) / 2.0; //Again a = 1.
}

vec3 normalSphere(in vec3 pos, in vec4 pr)
{
    return (pos - pr.xyz)/pr.w;
}

float intersectPlane(in vec3 ro, in vec3 rd, vec3 op)
{
    return ( (-ro.y + op.y) / rd.y);
}

vec3 normalPlane(in vec3 pos)
{
    return vec3(0.0, 1.0,  0.0);
}


// objects in the scene
int num_objects = 4;

vec3 obj_pos[]   = vec3[](vec3(0, -1.4, 0), vec3(0,1,1), vec3(-1.5, 1.2, 1.5), vec3(0.5,0.3,-0.1));
vec4 obj_prop[]  = vec4[](vec4(0), vec4(0.7), vec4(0.4), vec4(0.45));
vec3 obj_color[] = vec3[](vec3(0.5), vec3(1,0,0), vec3(0,1,0), vec3(0,0,1) );
int  obj_type[]  =  int[](0, 1, 1, 1); // 2 ellipse ...



    
    
    vec3 light = vec3(1,5,1);
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
    vec3 light_color = vec3(55);

              
              
bool RayIntersectsObject(int id, vec3 ro, vec3 rd, out float od, out vec3 op, out vec3 on)
{
    int type = obj_type[id];
    
    if(type == 0) // plane
    {
        od = intersectPlane(ro, rd, obj_pos[id]);
        
        if (od > 0.0)
        {
            op = ro + od * rd;
            on = normalPlane(op);
            //od = length(op - ro);
            return true;
        }
    }
    if(type == 1) // sphere
    {
        vec4 pr = vec4(obj_pos[id], obj_prop[id].x);
        od = intersectSphere( ro, rd, pr);
        
        if(od > 0.0)
        {
            op = ro + od * rd;
            on = normalSphere(op, pr);
            return true;
        }
    }

    return false;
}

vec4 chan3(float x){return texture(iChannel3,vec2(x,.5)/iResolution.xy,-100.);}


mat3 newCam( in vec3 o, in vec3 a) {
    vec3 w = normalize(a-o);
    vec3 p = vec3(0.0, 1.0, 0.0);
    vec3 u = normalize(cross(w,p));
    vec3 v = cross(u,w);
    return mat3( u, v, w );
}


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 mouse = iMouse.xy/iResolution.xy;
    vec3 wasd = chan3(.5).xyz;
    vec3 pos = vec3(mouse.x, mouse.y, 0.0);
    
    
    //!!!!!!!!!!!!!!!!!!!!!!!!!!task 1!!!!!!!!!!!
    if(texelFetch( iChannel0, ivec2(KEY_t,2),0 ).x <=0.0){
     obj_pos[1].xy += (mouse.xy - vec2(0.5))*10.;  
       
     obj_prop[1].x = mouse.x *1.5;
      
    }
    
     //obj_pos[1] .x = 0.5 * cos(iTime);
     //obj_pos[1] .z = 0.5 * sin(iTime);
     obj_pos[2] .x = 0.5 * cos(iTime);
     obj_pos[2] .z = 0.5 * sin(iTime);
     obj_pos[3] .x = 0.5 * cos(iTime);
     obj_pos[3] .z = 0.5 * sin(iTime);
     if(texelFetch( iChannel0, ivec2(KEY_b,2),0 ).x > 0.0){
        obj_prop[1].x = 0.5;
         obj_pos[1].xy = vec2(0.5);  

         obj_pos[1] .x = 0.3 ;
         obj_pos[1] .z = 0.1 ;
         obj_pos[2] .x = 0.6 ;
         obj_pos[2] .z = 0.5 ;
         obj_pos[3] .x = 0.9 ;
         obj_pos[3] .z = 0.9 ;
     }
    
    light.xz += vec2(sin(iTime), cos(iTime))*5.;
    
    
    // shading parameters
    float Ka = 0.3; // ambient
    float Kd = 0.7; // diffuse
    float Ks = 0.35; // specular
    
    // pixel coordinates from 0 to 1
    vec2 uv = (fragCoord.xy / iResolution.xy);
    
    // generate a ray with origin ro and direction rd!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    vec3 ro = vec3(0.0, 0.5, -15.0); // vec3(0,0,-1)
    vec3 vpp = wasd* 0.5 + vec3((-1.0 + uv * 2.0) * vec2(2.0, 2.0) * vec2(1.78*sin(iTime), 1.0 * sin(iTime)), 0);
    if(texelFetch( iChannel0, ivec2(KEY_r,2),0 ).x > 0.0){
        vpp = wasd* 0.5 + vec3((-1.0 + uv * 2.0) * vec2(2.0, 2.0) * vec2(1.78, 1.0), 0);
    
    }
    vec3 rd = normalize(vpp - ro);
    //vec3 rd_mouse = vpp + mouse.xy *rd ;
    
    // background color
    vec3 col = vec3(0.0);
    float intersection_dist = -1.0;
    
    
    //Rotate Camera
    if(texelFetch( iChannel0, ivec2(KEY_t,2),0 ).x > 0.0){
        vec2 uv = fragCoord/iResolution.xy;
        uv -= 0.5;
        uv.x *= iResolution.x/iResolution.y;
        
        vec3 col = vec3(0.0);
        vec2 mouse_tap = vec2(0.01) + iMouse.xy  / iResolution.xy ;
        mouse_tap -= 0.5;
        
        ro = vec3(3.0+ 8.0 * cos(mouse_tap.x * 2.0 * PI), 3.0 + 10.0 * mouse_tap.y, 10.0 * sin(mouse_tap.x *2.0 * PI));
        vec3 ta = vec3(0.0, 2.0, 0.0);
        mat3 cam = newCam(ro, ta);
        rd = normalize(cam * vec3(uv, 1.0));
    }
    

    // ray twds objects
    int obj_idx = -1;
    for(int i=0; i<num_objects; i++)
    {        
        float od;
        vec3 op, on;

        // ro = ray origin, rd = ray dir, od = obj dist, op = obj pos, on = obj normal
        if(RayIntersectsObject(i, ro, rd, od, op, on))
        {
            // first intersection
            if(intersection_dist < 0.0) 
            {
                intersection_dist = od;
            }
            else
            {
                // object is further away
                if(od > intersection_dist) continue;
                // object is closer than previous obj_pos[3] .x = 0.5 * cos(iTime);
    
                else intersection_dist = od;
            }
        }
    }
    

    
    
    // ray twds objects
    for(int i=0; i<num_objects; i++)
    {        
        float od;
        vec3 op, on;

        // ro = ray origin, rd = ray dir, od = obj dist, op = obj pos, on = obj normal
        if(RayIntersectsObject(i, ro, rd, od, op, on))
        {
            if(intersection_dist < 0.0) 
            {
                intersection_dist = od;
            }
            else
            {
                if(od > intersection_dist) continue;
                else intersection_dist = od;
            }
            
            // lambert shading
            vec3 L = normalize(light - op);
            float NdotL = dot(on, L);
            vec3 H = normalize( -rd + L);
            float NdotH = pow(dot(on, H), 46.0);
            vec3 I = light_color * 1.0 / pow(length(light - op), 2.0);
            vec3 col_ambient  = Ka * I * obj_color[i];
            vec3 col_diffuse  = Kd * I * obj_color[i] * max(0.0, NdotL);
            vec3 col_specular = Ks * I * vec3(1) * max(0.0, NdotH);
           

            // ray twds light source
            bool not_in_shadow = true;
            for(int j=0; j<num_objects; j++)
            {
                if(i == j) continue;
                
                float nothing_f;
                vec3 nothing_v;
                if(RayIntersectsObject(j, op, normalize(light-op), nothing_f, nothing_v, nothing_v))
                {
                    not_in_shadow = false;
                    break;
                }
            }
            
            col = col_ambient + col_diffuse * float(not_in_shadow) + col_specular;
        }
        
    }

    fragColor = vec4(col,1.0);
    //fragColor = vec4(mouse.xy,0,1.0)*1.;
   
}