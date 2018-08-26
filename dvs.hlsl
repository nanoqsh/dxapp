
struct Vertex
{
	float3 position : POSITION;
	float3 color : COLOR;
};

struct Output
{
	float4 position : SV_POSITION;
	float3 color : COLOR;
};

Output main(Vertex input)
{
	Output result;

	result.position = float4(input.position, 1.0F);
	result.color = input.color;

	return result;
}