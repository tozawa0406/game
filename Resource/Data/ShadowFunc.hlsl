
/* @brief	シャドウの生成	
 * @sa		PixelShader
 * @param	(positionLVP)	ライトのViewProj変換したポジション
 * @param	(tex)			シャドウマップ
 * @param	(texelSize)		テクセルサイズ
 * @return	影の濃さ*/
float CreateShadow(float4 positionLVP, float2 texelSize, float coefficient, Texture2D tex, SamplerComparisonState shadowSmp)
{
	// シャドウ
	float shadow = 0;

	// テクセルを計算
	float2 texel = float2(positionLVP.x / positionLVP.w * 0.5f + 0.5f, positionLVP.y / positionLVP.w * -0.5f + 0.5f);
	float inv = 1.0f / coefficient;

	// 深度マップの範囲外はとりあえず影なしにする
	if (texel.x < 0 || texel.x > 1 || texel.y < 0 || texel.y > 1) { shadow = 1.0f; }//inv; }
	else
	{
		float percentLit = 0;
		// ライトビュー上でのピクセルの深度値
		float depthcompare = positionLVP.z / positionLVP.w;
		// バイアス
		depthcompare -= 0.0002f;
		float cnt = 0;
		// 左右
		for (int i = -4; i < 5; i += 2)
		{
			// 上下
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

		// 周りのテクセルの平均値を取る
		percentLit = percentLit / cnt;
		shadow = clamp(percentLit, 0.5f, 1.0f);
//		shadow = coefficient * 0.1f;
	}

	return shadow;
}
