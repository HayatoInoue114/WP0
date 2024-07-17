float3x3 InverseTranspose(float3x3 input)
{
    float3x3 result = input;
    
    for (int i = 0; i < 3; i++)
    {
        result[i] /= length(result[i]);
    }
    
    return result;
}