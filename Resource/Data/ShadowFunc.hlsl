
/* @brief	�V���h�E�̐���	
 * @sa		PixelShader
 * @param	(positionLVP)	���C�g��ViewProj�ϊ������|�W�V����
 * @param	(tex)			�V���h�E�}�b�v
 * @param	(texelSize)		�e�N�Z���T�C�Y
 * @return	�e�̔Z��*/
float CreateShadow(float4 positionLVP, float2 texelSize, float coefficient, Texture2D tex, SamplerComparisonState shadowSmp)
{
	// �V���h�E
	float shadow = 0;

	// �e�N�Z�����v�Z
	float2 texel = float2(positionLVP.x / positionLVP.w * 0.5f + 0.5f, positionLVP.y / positionLVP.w * -0.5f + 0.5f);
	float inv = 1.0f / coefficient;

	// �[�x�}�b�v�͈̔͊O�͂Ƃ肠�����e�Ȃ��ɂ���
	if (texel.x < 0 || texel.x > 1 || texel.y < 0 || texel.y > 1) { shadow = 1.0f; }//inv; }
	else
	{
		float percentLit = 0;
		// ���C�g�r���[��ł̃s�N�Z���̐[�x�l
		float depthcompare = positionLVP.z / positionLVP.w;
		// �o�C�A�X
		depthcompare -= 0.0002f;
		float cnt = 0;
		// ���E
		for (int i = -4; i < 5; i += 2)
		{
			// �㉺
			for (int j = -4; j < 5; j += 2)
			{
				percentLit += tex.SampleCmpLevelZero(shadowSmp,
					float2(
						texel.x + (float)i * (texelSize.x * inv),
						texel.y + (float)j * (texelSize.y * inv)),
					depthcompare);
				cnt++;
			}
		}

		// ����̃e�N�Z���̕��ϒl�����
		percentLit = percentLit / cnt;
		shadow = clamp(percentLit, 0.5f, 1.0f);
//		shadow = coefficient * 0.1f;
	}

	return shadow;
}
