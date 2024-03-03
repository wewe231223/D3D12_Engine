#pragma once 

using MeshValue = std::pair<D3D12_VERTEX_BUFFER_VIEW, D3D12_INDEX_BUFFER_VIEW>;
using DescriptorHeapValue = D3D12_CPU_DESCRIPTOR_HANDLE;

using MeshParam = std::pair<std::vector<Vertex>, std::vector<UINT>>;
using PathParam = std::tstring;

using ResourceParameter = std::variant<MeshParam,PathParam>;
using ResourceContainTypes = std::variant<MeshValue, DescriptorHeapValue>;
