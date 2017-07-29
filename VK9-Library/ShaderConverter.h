/*
Copyright(c) 2016 Christopher Joseph Dean Schaefer

This software is provided 'as-is', without any express or implied
warranty.In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions :

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software.If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef SHADERCONVERTER_H
#define SHADERCONVERTER_H

#include <vulkan/vulkan.h>
#include <vector>
#include <boost/container/flat_map.hpp>

/*
http://timjones.io/blog/archive/2015/09/02/parsing-direct3d-shader-bytecode
https://msdn.microsoft.com/en-us/library/bb219840(VS.85).aspx#Shader_Binary_Format
http://stackoverflow.com/questions/2545704/format-of-compiled-directx9-shader-files
https://msdn.microsoft.com/en-us/library/windows/hardware/ff552891(v=vs.85).aspx
https://github.com/ValveSoftware/ToGL
*/

struct ConvertedShader
{
	UINT Size = 0;
	//VkVertexInputBindingDescription mVertexInputBindingDescription[16] = {};
	uint32_t VertexInputAttributeDescriptionCount = 0;
	VkVertexInputAttributeDescription mVertexInputAttributeDescription[32] = {};

	uint32_t mDescriptorSetLayoutBindingCount = 0;
	VkDescriptorSetLayoutBinding mDescriptorSetLayoutBinding[16] = {};

	VkShaderModule ShaderModule = VK_NULL_HANDLE;
};

class ShaderConverter
{
protected:
	VkDevice mDevice;
	ConvertedShader mConvertedShader;
public:
	ShaderConverter(VkDevice device);
	ConvertedShader Convert(uint32_t* shader);
private:	
	std::vector<uint32_t> mInstructions; //used to store the combined instructions for creating a module.
	boost::container::flat_map<uint32_t, uint32_t> mIdOffsetPairs;

	std::vector<uint32_t> mCapabilityInstructions;
	std::vector<uint32_t> mExtensionInstructions;
	std::vector<uint32_t> mImportExtendedInstructions;
	std::vector<uint32_t> mMemoryModelInstructions;
	std::vector<uint32_t> mEntryPointInstructions;
	std::vector<uint32_t> mExecutionModeInstructions;

	std::vector<uint32_t> mStringInstructions;
	std::vector<uint32_t> mSourceExtensionInstructions;
	std::vector<uint32_t> mSourceInstructions;
	std::vector<uint32_t> mSourceContinuedInstructions;
	std::vector<uint32_t> mNameInstructions;
	std::vector<uint32_t> mMemberNameInstructions;

	std::vector<uint32_t> mDecorateInstructions;
	std::vector<uint32_t> mMemberDecorateInstructions;
	std::vector<uint32_t> mGroupDecorateInstructions;
	std::vector<uint32_t> mGroupMemberDecorateInstructions;
	std::vector<uint32_t> mDecorationGroupInstructions;

	std::vector<uint32_t> mTypeInstructions;
	std::vector<uint32_t> mFunctionDeclarationInstructions;
	std::vector<uint32_t> mFunctionDefinitionInstructions;

	uint32_t* mBaseToken;
	uint32_t* mNextToken;
	uint32_t mBaseId;
	uint32_t mNextId;
	uint32_t mMinorVersion;
	uint32_t mMajorVersion;
	bool mIsVertexShader;

	uint32_t GetNextToken();
	void SkipTokens(uint32_t numberToSkip);
	uint32_t GetNextId();
	void SkipIds(uint32_t numberToSkip);
	uint32_t GetOpcode(uint32_t token);
	uint32_t GetOpcodeData(uint32_t token);
	uint32_t GetTextureType(uint32_t token);
	uint32_t GetRegisterType(uint32_t token);
	uint32_t GetRegisterNumber(uint32_t token);
	uint32_t GetUsage(uint32_t token);
	uint32_t GetUsageIndex(uint32_t token);

	void CombineSpirVOpCodes();
	void CreateSpirVModule();

};

#endif //SHADERCONVERTER_H
//
