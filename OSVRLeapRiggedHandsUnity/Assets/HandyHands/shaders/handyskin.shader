Shader "Custom/HandySkin" {
	Properties {
		_Color ("Main Color", Color) = (1,1,1,1)
		_Fresnel ("Fresnel", Range (.005, .1)) = .1
		_MainTex ("Diffuse (RGB) Alpha (A)", 2D) = "white" {}
		_BumpMap ("Normal (Normal)", 2D) = "bump" {}
		_RampTex ("Toon Ramp (RGB)", 2D) = "white" {}
	}

	SubShader{
		Tags { "RenderType" = "Opaque" }
		
		CGPROGRAM
			#pragma surface surf handyskin
			

			struct Input
			{
				float2 uv_MainTex;
				float3 viewDir;
				INTERNAL_DATA
			};
			
			sampler2D _MainTex, _BumpMap, _RampTex;
			float _Fresnel;
			float3 _Color;

			inline fixed4 Lightinghandyskin (SurfaceOutput s, fixed3 lightDir, fixed3 viewDir, fixed atten)
			{
				half3 h = normalize (lightDir + viewDir);
				fixed NdotL = dot(s.Normal, lightDir) * .5 + .5;
				fixed NdotH = saturate(dot(s.Normal, h));
				fixed NdotV = saturate(dot(s.Normal, viewDir));
				
				half specular_power = s.Specular * 128;
				half normalisation_term = ( specular_power + 2.0f ) / 8.0f;
				half blinn_phong = pow( NdotH, specular_power );
				half specular_term = normalisation_term * blinn_phong;
				half cosine_term = NdotL;
				half base = 1.0 - dot(h , viewDir);
				half exponential = pow( base, 5.0 );
				half fresnel_term = exponential + _Fresnel * ( 1.0 - exponential );
				half3 spec = specular_term * cosine_term * fresnel_term * _LightColor0.rgb * s.Specular;
				fixed3 ramp = tex2D(_RampTex, float2(NdotL, NdotL));
				
				fixed4 c;
				c.rgb = (s.Albedo * _LightColor0.rgb * ramp + _LightColor0.rgb * spec) * (atten * 2);
				c.a = 1;
				return c;
			}

			void surf (Input IN, inout SurfaceOutput o)
			{
				half4 tex = tex2D(_MainTex, IN.uv_MainTex);
				o.Albedo = tex.rgb * _Color;
				o.Normal = UnpackNormal(tex2D(_BumpMap, IN.uv_MainTex));
				o.Specular = tex.a;
			}
		ENDCG
	}
	Fallback "Bumped Specular"
}