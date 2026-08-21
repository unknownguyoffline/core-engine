#include "GLTFImporter.hpp"
#include "Core/Macro.hpp"
#include "json.hpp"
#include <fstream>

using namespace nlohmann;

namespace GLTF
{

enum class ComponentType
{
    None,
    Byte,
    UnsignedByte,
    Short,
    UnsignedShort,
    UnsignedInt,
    Float,
};

struct Accessor;

struct Extra
{
    std::string extensionsUsed;
    std::string extensionsRequired;
    std::vector<Accessor> accessor;

    // TODO: add all extra fields
};

struct SparseIndices
{
    uint32_t bufferView = UINT32_MAX;
    uint32_t byteOffset = 0;
    ComponentType componentType;

    json extension;
    Extra extra;
};

struct SparseValues
{
    uint32_t bufferView = UINT32_MAX;
    uint32_t byteOffset = 0;

    json extension;
    Extra extra;
};

struct Sparse
{
    uint32_t count = UINT32_MAX;

    // TODO: add all extra fields
};

struct AnimationChannelTarget
{
    uint32_t node = UINT32_MAX;
    std::string path;
    json extensions;
    Extra extras;
};

struct AnimationChannel
{
    uint32_t sampler = UINT32_MAX;
    AnimationChannelTarget target;
    json extensions;
    Extra extra;
};

struct AnimationSampler
{
    uint32_t input = UINT32_MAX;
    std::string interpolation = "LINEAR";
    uint32_t output = UINT32_MAX;
    json extensions;
    Extra extra;
};

struct Animation
{
    std::vector<AnimationChannel> channels;
    std::vector<AnimationSampler> sampler;
    std::string name;
    json extensions;
    Extra extras;
};

struct Asset
{
    std::string copyright;
    std::string generator;
    std::string version;
    std::string minVersion;
    json extensions;
    Extra extras;
};

struct Buffer
{
    std::string uri;
    uint32_t byteLength = UINT32_MAX;
    std::string name;
    json extensions;
    Extra extras;
};

struct BufferView
{
    uint32_t buffer = UINT32_MAX;
    uint32_t byteOffset = 0;
    uint32_t byteLength = UINT32_MAX;
    uint32_t byteStride = UINT32_MAX;
    uint32_t target = UINT32_MAX;
    std::string name;
    json extension;
    Extra extras;
};

struct CameraOrthographic
{
    float xmag = FLT_MAX;
    float ymag = FLT_MAX;
    float zfar = FLT_MAX;
    float znear = FLT_MAX;
    json extension;
    Extra extras;
};

struct CameraPerspective
{
    float aspectRatio = FLT_MAX;
    float yfov = FLT_MAX;
    float zfar = FLT_MAX;
    float znear = FLT_MAX;
    json extension;
    Extra extras;
};

struct Camera
{
    CameraOrthographic orthographic;
    CameraPerspective perspective;
    std::string type;
    std::string name;
    json extension;
    Extra extras;
};

struct Image
{
    std::string uri;
    std::string mimeType;
    uint32_t bufferView = UINT32_MAX;
    std::string name;
    json extension;
    Extra extras;
};

struct TextureInfo
{
    uint32_t index = UINT32_MAX;
    uint32_t texCoord = 0;
    json extension;
    Extra extras;
};

struct MaterialPbrMetallicRoughness
{
    glm::vec4 baseColorFactor = glm::vec4(1, 1, 1, 1);
    TextureInfo baseColorTexture;
    float metallicFactor = 1;
    float roughnessFactor = 1;
    TextureInfo metallicRoughnessTexture;
    json extension;
    Extra extras;
};

struct MaterialNormalTextureInfo
{
    uint32_t index = UINT32_MAX;
    uint32_t texCoord = 0;
    float scale = 1;
    json extension;
    Extra extras;
};

struct MaterialOcclusionTextureInfo
{
    uint32_t index = UINT32_MAX;
    uint32_t texCoord = 0;
    float strength = 1;
    json extension;
    Extra extra;
};

struct Material
{
    std::string name;
    MaterialPbrMetallicRoughness pbrMetallicRoughness;
    MaterialNormalTextureInfo normalTexture;
    MaterialOcclusionTextureInfo occlusionTexture;
    TextureInfo emissionTexture;
    glm::vec3 emissionFactor = glm::vec3(0);
    std::string alphaMode = "OPAQUE";
    float alphaCutOff = 0.5;
    bool doubleSided = false;

    json extension;
    Extra extra;
};

enum class MeshMode
{
    Points = 0,
    Lines = 1,
    LineLoop = 2,
    LineStrip = 3,
    Triangles = 4,
    TriangleStrip = 5,
    TriangleFan = 6
};

struct MeshPrimitive
{
    json attributes;
    uint32_t indices = UINT32_MAX;
    uint32_t material = UINT32_MAX;
    MeshMode mode = MeshMode::Triangles;
    std::vector<json> targets;
    json extension;
    Extra extras;
};

struct Mesh
{
    std::vector<MeshPrimitive> primitives;
    std::vector<float> weights;
    std::string name;
    json extension;
    Extra extras;
};

struct Node
{
    uint32_t camera = UINT32_MAX;
    std::vector<uint32_t> childrens;
    uint32_t skin = UINT32_MAX;
    glm::mat4 matrix = glm::mat4(1.f);
    uint32_t mesh = UINT32_MAX;
    glm::vec4 rotationQuat = glm::vec4(0, 0, 0, 1);
    glm::vec3 scale = glm::vec3(1, 1, 1);
    glm::vec3 translation = glm::vec3(0, 0, 0);
    std::vector<float> weights;
    std::string name;
    json extensions;
    Extra extras;
};

enum class MagFilter
{
    Nearest = 9728,
    Linear = 9729,
};

enum class MinFilter
{

    Nearest = 9728,
    Linear = 9729,
    NearestMipmapNearest = 9984,
    LinearMipmapNearest = 9985,
    NearestMipmapLinear = 9986,
    LinearMipmapLinear = 9987,
};

enum class SamplerWrap
{
    ClampToEdge = 33071,
    MirroredRepeat = 33648,
    Repeat = 10497,
};

struct Sampler
{
    MagFilter magFilter = MagFilter::Linear;
    MinFilter minFilter = MinFilter::Linear;
    SamplerWrap wrapS = SamplerWrap::Repeat;
    SamplerWrap wrapT = SamplerWrap::Repeat;
    std::string name;
    json extension;
    Extra extras;
};

struct Scene
{
    std::vector<uint32_t> nodes;
    std::string name;
    json extension;
    Extra extras;
};

struct Skin
{
    uint32_t inverseBindMatrices = UINT32_MAX;
    uint32_t skeleton = UINT32_MAX;
    std::vector<uint32_t> joints;
    std::string name;
    json extension;
    Extra extras;
};

struct Texture
{
    uint32_t sampler = UINT32_MAX;
    uint32_t source = UINT32_MAX;
    std::string name;
    json extension;
    Extra extras;
};

struct Accessor
{
    uint32_t bufferView = UINT32_MAX;
    uint32_t byteOffset = 0;
    ComponentType componentType = ComponentType::None;
    bool normalized = false;
    uint32_t count = UINT32_MAX;
    std::string type;
    void *max = nullptr;
    void *min = nullptr;
    Sparse sparse;
    std::string name;
    json extensions;
    Extra extra;
};

struct GLTFObjects
{
    Asset asset;
    std::vector<Accessor> accessors;
    std::vector<Animation> animations;
    std::vector<Buffer> buffers;
    std::vector<BufferView> bufferViews;
    std::vector<Camera> cameras;
    std::vector<Image> images;
    std::vector<Material> materials;
    std::vector<Mesh> meshes;
    std::vector<Node> nodes;
    std::vector<Sampler> samplers;
    std::vector<Scene> scenes;
    std::vector<Skin> skins;
    std::vector<Texture> textures;
    uint32_t scene = UINT32_MAX;

    json extensions;
    Extra extras;
};

void GetValue(const json &json, std::string_view field, glm::vec4 &value, bool required)
{
    if (!json.contains(field))
    {
        if (required)
        {
            ERROR("GLTFImporter: required field not found:- {}", field);
        }
        return;
    }

    value = {json[field][0], json[field][1], json[field][2], json[field][3]};
}

void GetValue(const json &json, std::string_view field, glm::vec3 &value, bool required)
{
    if (!json.contains(field))
    {
        if (required)
        {
            ERROR("GLTFImporter: required field not found:- {}", field);
        }
        return;
    }

    value = {json[field][0], json[field][1], json[field][2]};
}

void GetValue(const json &json, std::string_view field, glm::vec2 &value, bool required)
{
    if (!json.contains(field))
    {
        if (required)
        {
            ERROR("GLTFImporter: required field not found:- {}", field);
        }
        return;
    }

    value = {json[field][0], json[field][1]};
}

template <typename type>
void GetValue(const json &json, std::string_view field, type &value, bool required)
{
    if (!json.contains(field))
    {
        if (required)
        {
            ERROR("GLTFImporter: required field not found:- {}", field);
        }
        return;
    }

    value = json[field];
}

Extra GetExtras(json &json)
{
    return Extra();
}

Buffer GetBuffer(const json &json)
{
    Buffer buffer;
    GetValue<std::string>(json, "uri", buffer.uri, false);
    GetValue<uint32_t>(json, "byteLength", buffer.byteLength, true);
    GetValue<std::string>(json, "name", buffer.name, false);

    return buffer;
}

BufferView GetBufferView(const json &json)
{
    BufferView view;

    GetValue<uint32_t>(json, "buffer", view.buffer, true);
    GetValue<uint32_t>(json, "byteOffset", view.byteOffset, false);
    GetValue<uint32_t>(json, "byteLength", view.byteLength, true);
    GetValue<uint32_t>(json, "byteStride", view.byteStride, false);
    GetValue<uint32_t>(json, "target", view.target, false);
    GetValue<std::string>(json, "name", view.name, false);

    return view;
}

CameraOrthographic GetCameraOrthographic(const json &json)
{
    CameraOrthographic camera;

    GetValue<float>(json, "xmag", camera.xmag, true);
    GetValue<float>(json, "ymag", camera.ymag, true);
    GetValue<float>(json, "zfar", camera.zfar, true);
    GetValue<float>(json, "znear", camera.znear, true);

    return camera;
}

CameraPerspective GetCameraPerspective(const json &json)
{
    CameraPerspective camera;

    GetValue<float>(json, "aspectRatio", camera.aspectRatio, false);
    GetValue<float>(json, "yfov", camera.yfov, true);
    GetValue<float>(json, "zfar", camera.zfar, false);
    GetValue<float>(json, "znear", camera.znear, true);

    return camera;
}

Camera GetCamera(const json &json)
{
    Camera camera;

    if (json.contains("perspective"))
    {
        camera.perspective = GetCameraPerspective(json["perspective"]);
    }

    if (json.contains("orthographic"))
    {
        camera.orthographic = GetCameraOrthographic(json["orthographic"]);
    }

    GetValue<std::string>(json, "type", camera.type, true);
    GetValue<std::string>(json, "name", camera.name, false);

    return camera;
}

Image GetImage(const json &json)
{
    Image image;
    GetValue<std::string>(json, "uri", image.uri, false);
    GetValue<std::string>(json, "mimeType", image.mimeType, false);
    GetValue<uint32_t>(json, "bufferView", image.bufferView, false);
    GetValue<std::string>(json, "name", image.name, false);

    return image;
}

TextureInfo GetTextureInfo(const json &json)
{
    TextureInfo textureInfo;
    GetValue<uint32_t>(json, "index", textureInfo.index, true);
    GetValue<uint32_t>(json, "texCoord", textureInfo.texCoord, false);

    return textureInfo;
}

MaterialPbrMetallicRoughness GetMaterialPbrMetallicRoughness(const json &json)
{
    MaterialPbrMetallicRoughness pbr;
    glm::vec4 baseColorFactor = glm::vec4(1, 1, 1, 1);

    GetValue(json, "baseColorFactor", pbr.baseColorFactor, false);

    if (json.contains("baseColorTexture"))
    {
        pbr.baseColorTexture = GetTextureInfo(json["baseColorTexture"]);
    }

    GetValue<float>(json, "metallicFactor", pbr.metallicFactor, false);
    GetValue<float>(json, "roughnessFactor", pbr.roughnessFactor, false);

    if (json.contains("metallicRoughnessTexture"))
    {
        pbr.metallicRoughnessTexture = GetTextureInfo(json["metallicRoughnessTexture"]);
    }

    return pbr;
}

MaterialOcclusionTextureInfo GetMaterialOcclusionTextureInfo(const json &json)
{
    MaterialOcclusionTextureInfo occlusion;
    GetValue<uint32_t>(json, "index", occlusion.index, true);
    GetValue<uint32_t>(json, "texCoord", occlusion.texCoord, false);
    GetValue<float>(json, "strength", occlusion.strength, false);

    return occlusion;
}

MaterialNormalTextureInfo GetMaterialNormalTextureInfo(const json &json)
{
    MaterialNormalTextureInfo normal;
    GetValue<uint32_t>(json, "index", normal.index, true);
    GetValue<uint32_t>(json, "texCoord", normal.texCoord, false);
    GetValue<float>(json, "scale", normal.scale, false);

    return normal;
}

Material GetMaterial(const json &json)
{
    Material material;

    GetValue<std::string>(json, "name", material.name, false);

    if (json.contains("pbrMetallicRoughness"))
    {
        material.pbrMetallicRoughness = GetMaterialPbrMetallicRoughness(json["pbrMetallicRoughness"]);
    }

    if (json.contains("normalTexture"))
    {
        material.normalTexture = GetMaterialNormalTextureInfo(json["normalTexture"]);
    }

    if (json.contains("occlusionTexture"))
    {
        material.occlusionTexture = GetMaterialOcclusionTextureInfo(json["occlusionTexture"]);
    }

    if (json.contains("emissionTexture"))
    {
        material.emissionTexture = GetTextureInfo(json["emissionTexture"]);
    }

    GetValue(json, "emissionFactor", material.emissionFactor, false);
    GetValue<std::string>(json, "alphaMode", material.alphaMode, false);
    GetValue<float>(json, "alphaCutoff", material.alphaCutOff, false);
    GetValue<bool>(json, "doubleSided", material.doubleSided, false);

    return material;
}

MeshPrimitive GetMeshPrimitive(const json &json)
{
    MeshPrimitive primitive;

    GetValue<::json>(json, "attributes", primitive.attributes, true);
    GetValue<uint32_t>(json, "indices", primitive.indices, false);
    GetValue<uint32_t>(json, "material", primitive.material, false);
    GetValue<MeshMode>(json, "mode", primitive.mode, false);

    // TODO: parse targets

    return primitive;
}

Mesh GetMesh(const json &json)
{
    Mesh mesh;

    json::array_t primitivesJson = json["primitives"];
    for (const ::json &primitiveJson : primitivesJson)
    {
        mesh.primitives.push_back(GetMeshPrimitive(primitiveJson));
    }

    if (json.contains("weights"))
    {

        json::array_t weights = json["weights"];
        for (float weight : weights)
        {
            mesh.weights.push_back(weight);
        }
    }

    GetValue<std::string>(json, "name", mesh.name, false);

    return mesh;
}

Sampler GetSampler(const json &json)
{
    Sampler sampler;

    GetValue<MagFilter>(json, "magFilter", sampler.magFilter, false);
    GetValue<MinFilter>(json, "minFilter", sampler.minFilter, false);
    GetValue<SamplerWrap>(json, "wrapS", sampler.wrapS, false);
    GetValue<SamplerWrap>(json, "wrapT", sampler.wrapT, false);
    GetValue<std::string>(json, "name", sampler.name, false);

    return sampler;
}

Scene GetScene(const json &json)
{
    Scene scene;

    if (json.contains("nodes"))
    {
        json::array_t nodeJson = json["nodes"];
        for (uint32_t node : nodeJson)
        {
            scene.nodes.push_back(node);
        }
    }

    GetValue<std::string>(json, "name", scene.name, false);

    return scene;
}

Skin GetSkin(const json &json)
{
    Skin skin;

    GetValue<uint32_t>(json, "inverseBindMatrices", skin.inverseBindMatrices, false);
    GetValue<uint32_t>(json, "skeleton", skin.skeleton, false);

    json::array_t jointsJson = json["joints"];
    for (uint32_t joint : jointsJson)
    {
        skin.joints.push_back(joint);
    }

    GetValue<std::string>(json, "name", skin.name, false);

    return skin;
}

Texture GetTexture(const json &json)
{
    Texture texture;

    GetValue<uint32_t>(json, "sampler", texture.sampler, false);
    GetValue<uint32_t>(json, "source", texture.source, false);
    GetValue<std::string>(json, "name", texture.name, false);

    return texture;
}

Asset GetAsset(const json &json)
{
    Asset assets;

    GetValue<std::string>(json, "copyright", assets.copyright, false);
    GetValue<std::string>(json, "generator", assets.generator, false);
    GetValue<std::string>(json, "version", assets.version, true);
    GetValue<std::string>(json, "minVersion", assets.minVersion, false);

    return assets;
}

Accessor GetAccessor(const json &json)
{
    Accessor accessor;

    GetValue<uint32_t>(json, "bufferView", accessor.bufferView, false);
    GetValue<uint32_t>(json, "byteOffset", accessor.byteOffset, false);
    GetValue<ComponentType>(json, "componentType", accessor.componentType, false);
    GetValue<bool>(json, "normalized", accessor.normalized, false);
    GetValue<uint32_t>(json, "count", accessor.count, false);
    GetValue<std::string>(json, "type", accessor.type, false);
    GetValue<std::string>(json, "name", accessor.name, false);

    // TODO: Parse sparse

    return accessor;
}

Animation GetAnimation(const json &json)
{
    Animation animation;

    // TODO: Parse animation

    return animation;
}

Node GetNode(const json &json)
{
    Node node;
    std::vector<uint32_t> childrens;

    uint32_t camera = UINT32_MAX;
    uint32_t skin = UINT32_MAX;
    glm::mat4 matrix = glm::mat4(1.f);
    uint32_t mesh = UINT32_MAX;
    glm::vec4 rotationQuat = glm::vec4(0, 0, 0, 1);
    glm::vec3 scale = glm::vec3(1, 1, 1);
    glm::vec3 translation = glm::vec3(0, 0, 0);
    std::vector<float> weights;
    std::string name;

    return node;
}

template <typename structure>
std::vector<structure> GetObjectList(const json &json, std::string_view field, std::function<structure(const ::json &)> parseCallback)
{
    std::vector<structure> objects;

    if (json.contains(field))
    {
        json::array_t objectJsons = json[field];
        for (const ::json &objectJson : objectJsons)
        {
            objects.push_back(parseCallback(objectJson));
        }
    }

    return objects;
}

} // namespace GLTF

void GLTFImporter::Import(std::string_view filename, Scene &scene)
{
    std::ifstream input(filename.data());
    if (!input.is_open())
    {
        ERROR("Failed to open file: {}", filename);
        return;
    }

    json json = json::parse(input);

    GLTF::GLTFObjects objects;

    objects.accessors = GLTF::GetObjectList<GLTF::Accessor>(json, "accessors", [](const ::json &objectJson) {
        return GLTF::GetAccessor(objectJson);
    });

    objects.buffers = GLTF::GetObjectList<GLTF::Buffer>(json, "buffers", [](const ::json &objectJson) {
        return GLTF::GetBuffer(objectJson);
    });

    objects.animations = GLTF::GetObjectList<GLTF::Animation>(json, "animations", [](const ::json &objectJson) {
        return GLTF::GetAnimation(objectJson);
    });
    objects.bufferViews = GLTF::GetObjectList<GLTF::BufferView>(json, "bufferViews", [](const ::json &objectJson) {
        return GLTF::GetBufferView(objectJson);
    });
    objects.cameras = GLTF::GetObjectList<GLTF::Camera>(json, "cameras", [](const ::json &objectJson) {
        return GLTF::GetCamera(objectJson);
    });
    objects.images = GLTF::GetObjectList<GLTF::Image>(json, "images", [](const ::json &objectJson) {
        return GLTF::GetImage(objectJson);
    });
    objects.materials = GLTF::GetObjectList<GLTF::Material>(json, "materials", [](const ::json &objectJson) {
        return GLTF::GetMaterial(objectJson);
    });
    objects.meshes = GLTF::GetObjectList<GLTF::Mesh>(json, "meshes", [](const ::json &objectJson) {
        return GLTF::GetMesh(objectJson);
    });
    objects.nodes = GLTF::GetObjectList<GLTF::Node>(json, "nodes", [](const ::json &objectJson) {
        return GLTF::GetNode(objectJson);
    });
    objects.samplers = GLTF::GetObjectList<GLTF::Sampler>(json, "samplers", [](const ::json &objectJson) {
        return GLTF::GetSampler(objectJson);
    });
    objects.scenes = GLTF::GetObjectList<GLTF::Scene>(json, "scenes", [](const ::json &objectJson) {
        return GLTF::GetScene(objectJson);
    });
    objects.skins = GLTF::GetObjectList<GLTF::Skin>(json, "skins", [](const ::json &objectJson) {
        return GLTF::GetSkin(objectJson);
    });
    objects.textures = GLTF::GetObjectList<GLTF::Texture>(json, "textures", [](const ::json &objectJson) {
        return GLTF::GetTexture(objectJson);
    });
}
