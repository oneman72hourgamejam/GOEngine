//uniform sampler2D qt_Texture0;
//varying highp vec2 qt_TexCoord0;

uniform sampler2D u_texture;
// положение источника света (хайпресижн)
uniform highp vec4 u_lightPosition;
uniform highp vec4 u_lightColor;
uniform highp float u_lightPower;
uniform highp float u_specularFactor;
uniform highp float u_ambientFactor;
varying highp vec4 v_position;
varying highp vec2 v_texcoord;
varying highp vec3 v_normal;

void main(void)
{
    //gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
    //gl_FragColor = texture2D(qt_Texture0, qt_TexCoord0);

    // результирующий цвет (черный)
    vec4 resultColor = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 eyePosition = vec4(0.0, 0.0, 0.0, 1.0);
    // сохраняем дифьюзный цвет материала
    vec4 diffMatColor = texture2D(u_texture, v_texcoord);
    // вектор направления взгляда
    // направлен из точки 0, 0, 0 в точку которую обрабатывает фрагментный шейдер
    vec3 eyeVect = normalize(v_position.xyz - eyePosition.xyz);
    // направлен из точки где находится источник освещения к вектору позиции
    vec3 lightVect = normalize(v_position.xyz - u_lightPosition);
    // отраженный свет, вектор направленный из точки которую рассматриваем в направлении куда отразится свет
    vec3 reflectLight = normalize(reflect(lightVect, v_normal));
    // расстояние от наблюдателя к рассматриваемой точке
    float len = length(v_position.xyz - eyePosition.xyz);
    // отвечает за то, насколько большое будет пятно блика
    float specularFactor = u_specularFactor;
    // отвечает за то, насколько будет светиться сам материал
    float ambientFactor = u_ambientFactor;

    vec4 diffColor = diffMatColor * u_lightPower * max(0.0, dot(v_normal, -lightVect)) / (1.0 + 0.25 * pow(len, 2.0));
    resultColor += diffColor;
    vec4 ambientColor = ambientFactor * diffMatColor;
    resultColor += ambientColor;
    // делим на 1 + коефициент * на квадрат расстояния, чтобы с отдалением было меньше освещения и влияния источника света
    vec4 specularColor = u_lightColor * u_lightPower * pow(max(0.0, dot(reflectLight, -eyeVect)), specularFactor) / (1.0 + 0.25 * pow(len, 2.0));
    resultColor += specularColor;

    gl_FragColor = resultColor;
}
