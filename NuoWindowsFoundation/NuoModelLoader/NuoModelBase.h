//
//  NuoModelBase.hpp
//  ModelViewer
//
//  Created by middleware on 8/28/16.
//  Copyright © 2016 middleware. All rights reserved.
//

#ifndef NuoModelBase_hpp
#define NuoModelBase_hpp


#include <memory>
#include <vector>
#include <string>

#include "NuoUtilites/NuoMathVector.h"


class NuoModelBase : public std::enable_shared_from_this<NuoModelBase>
{
protected:
    std::string _name;
    std::vector<uint32_t> _indices;

public:

    virtual void AddPosition(size_t sourceIndex, const std::vector<float>& positionsBuffer) = 0;
    virtual void AddNormal(size_t sourceIndex, const std::vector<float>& normalBuffer) = 0;
    virtual void AddTexCoord(size_t sourceIndex, const std::vector<float>& texCoordBuffer) = 0;

    virtual void GenerateIndices() = 0;
    virtual void GenerateNormals() = 0;

    virtual size_t GetVerticesNumber() const = 0;
    virtual size_t GetIndicesNumber() const = 0;
    virtual NuoVectorFloat4 GetPosition(size_t index) = 0;

    virtual void* Ptr() = 0;
    virtual size_t Length() = 0;
    virtual uint32_t* IndicesPtr();
    virtual size_t IndicesLength();
    virtual size_t IndicesCount();

    virtual const std::string& GetName() const = 0;
  
};



template <class ItemBase>
class NuoModelCommon : virtual public NuoModelBase
{
protected:
    std::vector<ItemBase> _buffer;

public:
    virtual void AddPosition(size_t sourceIndex, const std::vector<float>& positionsBuffer) override;
    virtual void AddNormal(size_t sourceIndex, const std::vector<float>& normalBuffer) override;

    virtual void GenerateIndices() override;
    virtual void GenerateNormals() override;

    virtual size_t GetVerticesNumber() const override;
    virtual size_t GetIndicesNumber() const override;
    virtual NuoVectorFloat4 GetPosition(size_t index) override;

    virtual void* Ptr() override;
    virtual size_t Length() override;

    virtual void SetName(const std::string& name);
    virtual const std::string& GetName() const override;

protected:
    virtual void DoGenerateIndices(bool compressBuffer);
};



struct NuoItemSimple
{
    NuoVectorFloat4::_typeTrait::_vectorType _position;
    NuoVectorFloat4::_typeTrait::_vectorType _normal;

    NuoItemSimple();

    bool operator == (const NuoItemSimple& other);
};



class NuoModelSimple : virtual public NuoModelCommon<NuoItemSimple>
{
protected:


public:
    NuoModelSimple();

    virtual void AddTexCoord(size_t sourceIndex, const std::vector<float>& texCoordBuffer) override;

};



template <class ItemBase>
void NuoModelCommon<ItemBase>::GenerateIndices()
{
    DoGenerateIndices(true);
}



template <class ItemBase>
void NuoModelCommon<ItemBase>::DoGenerateIndices(bool compressBuffer)
{
    uint32_t indexCurrent = 0;

    if (compressBuffer)
    {
        std::vector<ItemBase> compactBuffer;
        size_t checkBackward = 200;

        _indices.clear();

        for (size_t i = 0; i < _buffer.size(); ++i)
        {
            const ItemBase& item = _buffer[i];

            NuoVectorFloat4 normal = _buffer[i]._normal;
            if (normal.x() != 0.0f || normal.y() != 0.0f || normal.z() != 0.0f)
            {
                auto search = std::find((compactBuffer.size() < checkBackward ? compactBuffer.begin() : compactBuffer.end() - checkBackward),
                                         compactBuffer.end(), item);
                if (search != std::end(compactBuffer))
                {
                    uint32_t indexExist = (uint32_t)(search - std::begin(compactBuffer));
                    _indices.push_back(indexExist);
                }
                else
                {
                    compactBuffer.push_back(item);
                    _indices.push_back(indexCurrent++);
                }
            }
            else
            {
                compactBuffer.push_back(item);
                _indices.push_back(indexCurrent++);
            }
        }

        _buffer.swap(compactBuffer);
    }
    else
    {
        for (size_t i = 0; i < _buffer.size(); ++i)
            _indices.push_back(indexCurrent++);
    }
}


template <class ItemBase>
void NuoModelCommon<ItemBase>::GenerateNormals()
{
    size_t indexCount = _indices.size();
    for (size_t i = 0; i < indexCount; i += 3)
    {
        uint32_t i0 = _indices[i];
        uint32_t i1 = _indices[i + 1];
        uint32_t i2 = _indices[i + 2];

        ItemBase* v0 = &_buffer[i0];
        ItemBase* v1 = &_buffer[i1];
        ItemBase* v2 = &_buffer[i2];

        NuoVectorFloat4 nv0 = v0->_position;
        NuoVectorFloat4 nv1 = v1->_position;
        NuoVectorFloat4 nv2 = v2->_position;

        const NuoVectorFloat3 p0(nv0.x(), nv0.y(), nv0.z());
        const NuoVectorFloat3 p1(nv1.x(), nv1.y(), nv1.z());
        const NuoVectorFloat3 p2(nv2.x(), nv2.y(), nv2.z());

        const NuoVectorFloat3 cross = NuoCross((p1 - p0), (p2 - p0));
        const NuoVectorFloat4 cross4(cross.x(), cross.y(), cross.z(), 0);

        v0->_normal = (NuoVectorFloat4(v0->_normal) + NuoVectorFloat4(cross4._vector))._vector;
        v1->_normal = (NuoVectorFloat4(v1->_normal) + NuoVectorFloat4(cross4._vector))._vector;
        v2->_normal = (NuoVectorFloat4(v2->_normal) + NuoVectorFloat4(cross4._vector))._vector;
    }

    for (size_t i = 0; i < _buffer.size(); ++i)
    {
        _buffer[i]._normal = NuoVectorFloat4::Normalize(_buffer[i]._normal);
    }
}


template <class ItemBase>
void NuoModelCommon<ItemBase>::AddPosition(size_t sourceIndex, const std::vector<float>& positionsBuffer)
{
    size_t sourceOffset = sourceIndex * 3;

    ItemBase newItem;

    newItem._position = NuoVectorFloat4(positionsBuffer[sourceOffset],
                                        positionsBuffer[sourceOffset + 1],
                                        positionsBuffer[sourceOffset + 2], 1.0f)._vector;

    _buffer.push_back(newItem);
}



template <class ItemBase>
void NuoModelCommon<ItemBase>::AddNormal(size_t sourceIndex, const std::vector<float>& normalBuffer)
{
    size_t sourceOffset = sourceIndex * 3;
    size_t targetOffset = _buffer.size() - 1;

    _buffer[targetOffset]._normal = NuoVectorFloat4(normalBuffer[sourceOffset],
                                                    normalBuffer[sourceOffset + 1],
                                                    normalBuffer[sourceOffset + 2], 0.0f)._vector;
}



template <class ItemBase>
size_t NuoModelCommon<ItemBase>::GetVerticesNumber() const
{
    return _buffer.size();
}


template <class ItemBase>
size_t NuoModelCommon<ItemBase>::GetIndicesNumber() const
{
    return _indices.size();
}



template <class ItemBase>
NuoVectorFloat4 NuoModelCommon<ItemBase>::GetPosition(size_t index)
{
    return _buffer[index]._position;
}


template <class ItemBase>
void* NuoModelCommon<ItemBase>::Ptr()
{
    return (void*)_buffer.data();
}


template <class ItemBase>
size_t NuoModelCommon<ItemBase>::Length()
{
    return _buffer.size() * sizeof(ItemBase);
}



template <class ItemBase>
void NuoModelCommon<ItemBase>::SetName(const std::string& name)
{
    _name = name;
}



template <class ItemBase>
const std::string& NuoModelCommon<ItemBase>::GetName() const
{
    return _name;
}




#endif /* NuoModelBase_hpp */
