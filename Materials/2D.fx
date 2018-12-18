texture Texture;
sampler DecaleSamp = sampler_state
{
    Texture = <Texture>;
    MinFilter = POINT;//LINEAR;
    MagFilter = POINT;//LINEAR;
    MipFilter = NONE;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

// -------------------------------------------------------------
// ���_�V�F�[�_����s�N�Z���V�F�[�_�ɓn���f�[�^
// -------------------------------------------------------------
struct VStoPS
{
    float4 Pos			: POSITION;
	float2 Tex			: TEXCOORD0;
    float4 Color		: COLOR0;
};


////////////////////////////////////////////////////////////////
//                         ������                              //
//                                                            //
//           ���V�F�[�_�̖��O�͕K���T�����ȓ��ɂ��邱��              //
//                                                            //
////////////////////////////////////////////////////////////////

// -------------------------------------------------------------
// �s�N�Z���V�F�[�_�v���O����
// -------------------------------------------------------------
float4 PS_Copy(VStoPS In) : COLOR
{
	float4 OUT;
	OUT = tex2D(DecaleSamp, In.Tex);
	return OUT;
}

technique copy
{
    pass P0
    {
		AlphaBlendEnable = true;
		BlendOp          = Add;
		SrcBlend         = SrcAlpha;
		DestBlend        = InvSrcAlpha;
		// �V�F�[�_
        PixelShader  = compile ps_3_0 PS_Copy();
    }
}


// -------------------------------------------------------------
// �s�N�Z���V�F�[�_�v���O����
// -------------------------------------------------------------
float zoom_rate;
float zoom_x = 0.5; //	�Y�[���̒��S
float zoom_y = 0.5;

float4 PS_Zoom(VStoPS In) : COLOR{
	float4 OUT;
	float2 tex = In.Tex;
	tex.x -= zoom_x;
	tex.y -= zoom_y;
	tex /= zoom_rate; // zoom_rate�{�Y�[��
	tex.x += zoom_x;
	tex.y += zoom_y;
	OUT = tex2D(DecaleSamp, tex);
	return OUT;
}
technique zoom
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		// �V�F�[�_
		PixelShader = compile ps_3_0 PS_Zoom();
	}
}

// -------------------------------------------------------------
// Fade�V�F�[�_�͂�����
// -------------------------------------------------------------
float FadeRate;
float4 PS_Fade(VStoPS In) : COLOR
{
float4 OUT;
OUT = tex2D(DecaleSamp, In.Tex);
//OUT.a = FadeRate;
if (OUT.r >= FadeRate) {
	OUT.a = 0;
}

if (OUT.r >= FadeRate - 0.05) {
	OUT.r = 1;
	OUT.g = 0;
	OUT.b = 0;
}
else {
	OUT.rgb = 0;
}
OUT.rgb = 0;
return OUT;
}

technique fade
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		// �V�F�[�_
		PixelShader = compile ps_3_0 PS_Fade();
	}
}

// -------------------------------------------------------------
// ���Î�`�͂�����
// -------------------------------------------------------------
float4 PS_Sepia(VStoPS In) : COLOR
{
	float4 OUT;
OUT = tex2D(DecaleSamp, In.Tex);
OUT.rgb = OUT.r + OUT.g + OUT.b / 3;
OUT.r *= 1.2;
OUT.g *= 0.7;
OUT.b *= 0.4;
return OUT;
}

technique sepia
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		// �V�F�[�_
		PixelShader = compile ps_3_0 PS_Sepia();
	}
}

// -------------------------------------------------------------
// �򕨂͂�����
// -------------------------------------------------------------
float GreenX, GreenY;
float4 PS_Diz(VStoPS In) : COLOR
{
	float4 OUT;
OUT = tex2D(DecaleSamp, In.Tex);
OUT.r = tex2D(DecaleSamp, In.Tex);
OUT.g = tex2D(DecaleSamp, In.Tex + float2(GreenX, -GreenY));
OUT.b = tex2D(DecaleSamp, In.Tex + float2(-GreenX,GreenY));
return OUT;
}

technique diz
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		// �V�F�[�_
		PixelShader = compile ps_3_0 PS_Diz();
	}
}

// -------------------------------------------------------------
// DIO�͂�����
// -------------------------------------------------------------
float NegaFade;//�ŏ���1.0f����0.2������
float4 PS_Nega(VStoPS In) : COLOR
{
	float4 OUT;
//if (NegaFade < 0.0) {
//	NegaFade = 0.0;
//}
OUT = tex2D(DecaleSamp, In.Tex);
//OUT.r = 255 - tex2D(DecaleSamp, In.Tex);
//OUT.g = 255 - tex2D(DecaleSamp, In.Tex);
//OUT.b = 255 - tex2D(DecaleSamp, In.Tex);
OUT.rgb = 1.0f - OUT.rgb + NegaFade;
return OUT;
}

technique nega
{
	pass P0
	{
		AlphaBlendEnable = true;
		BlendOp = Add;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		// �V�F�[�_
		PixelShader = compile ps_3_0 PS_Nega();
	}
}


