#include "modelClass.h"

ModelClass::ModelClass()
{
	vertexBuffer_ = 0;
	indexBuffer_ = 0;
	texture_ = 0;
	cal3dCoreModel_ = 0;
	cal3dModel_ = 0;
	animationToPlay_ = "";
}

ModelClass::ModelClass(const ModelClass& other)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::setup(ID3D11Device* device, char* modelName)
{
	bool result;

	cal3dCoreModel_ = new CalCoreModel(modelName);
	if(!cal3dCoreModel_)
	{
		return false;
	}

	if(!parseBasicModelConfiguration(modelName))
	{
		return false;
	}

	// Initialize the vertex and index buffer that hold the geometry for the triangle.
	result = setupBuffers(device);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = loadTexture(device, modelName);
	if(!result)
	{
		return false;
	}

	return true;
}

void ModelClass::update(float elapsedTime)
{
	//cal3dModel_->getMixer()->world_rotation.set( CalVector( 0,1,0 ), model_yaw );
	cal3dModel_->update(elapsedTime);
}

void ModelClass::draw(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT result = updateBuffers(deviceContext);
	if(!result)
	{
		return;
	}

	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	drawBuffers(deviceContext);

	return;
}

void ModelClass::destroy()
{
	// Release the textrure
	destroyTexture();

	// Release the vertex and index buffers.
	destroyBuffers();

	// Release the model
	destroyModel();

	return;
}

int ModelClass::getIndexCount()
{
	return indexCount_;
}

ID3D11ShaderResourceView* ModelClass::getTexture()
{
	return texture_->getTexture();
}

void ModelClass::decreaseAnimationToDisplay()
{
	std::map<std::string, int>::iterator animIterator;
	animIterator = animations_.find(animationToPlay_);
	if(animIterator == animations_.begin())
	{
		animIterator = animations_.end();
		animIterator--;
		animationToPlay_ = animIterator->first;
		if(animationToPlay_ != initialAnimationToPlay_)
		{
			cal3dModel_->getMixer()->executeAction(animIterator->second, 0.25f, 0.25f, 1.0f );
		}
		else
		{
			cal3dModel_->getMixer()->removeAction(animations_.at(initialAnimationToPlay_));
		}
	}
	else
	{
		animIterator--;
		animationToPlay_ = animIterator->first;
		if(animationToPlay_ != initialAnimationToPlay_)
		{
			cal3dModel_->getMixer()->executeAction(animIterator->second, 0.25f, 0.25f, 1.0f );
		}
		else
		{
			cal3dModel_->getMixer()->removeAction(animations_.at(initialAnimationToPlay_));
		}
	}
}

void ModelClass::increaseAnimationToDisplay()
{
	std::map<std::string, int>::iterator animIterator;
	animIterator = animations_.find(animationToPlay_);
	animIterator++;
	if(animIterator == animations_.end())
	{
		animIterator = animations_.begin();
		animationToPlay_ = animIterator->first;
		if(animationToPlay_ != initialAnimationToPlay_)
		{
			cal3dModel_->getMixer()->executeAction(animIterator->second, 0.25f, 0.25f, 1.0f );
		}
		else
		{
			cal3dModel_->getMixer()->removeAction(animations_.at(initialAnimationToPlay_));
		}
	}
	else
	{
		animationToPlay_ = animIterator->first;
		if(animationToPlay_ != initialAnimationToPlay_)
		{
			cal3dModel_->getMixer()->executeAction(animIterator->second, 0.25f, 0.25f, 1.0f );
		}
		else
		{
			cal3dModel_->getMixer()->removeAction(animations_.at(initialAnimationToPlay_));
		}
	}
}

bool ModelClass::setupBuffers(ID3D11Device* device)
{
	VertexType *vertices = 0, *verticesSubmesh = 0;
	CalIndex *indices = 0, *indicesSubmesh = 0;
	int vertexCountSubmesh, indexCountSubmesh;
	D3D11_BUFFER_DESC vertex_buffer_desc, index_buffer_desc;
	D3D11_SUBRESOURCE_DATA vertex_data, index_data;
	HRESULT result;

	//Release the old buffers to create the new ones
	if(vertexBuffer_)
		vertexBuffer_->Release();

	if(indexBuffer_)
		indexBuffer_->Release();

	vertexCount_ = 0;
	indexCount_ = 0;

	//Start loading information
	CalRenderer *cal3dRenderer = cal3dModel_->getRenderer();

	// begin the rendering loop
	if(!cal3dRenderer->beginRendering()){
		int errorCode = CalError::getLastErrorCode();
		std::string errorString = CalError::getLastErrorDescription();
		std::string errorFile = CalError::getLastErrorFile();
		int errorLine = CalError::getLastErrorLine();
		std::stringstream errorStream;
		errorStream << "Error number " << errorCode << ": " << errorString << ". File: " << errorFile << " line " << errorLine;
		MessageBoxA(NULL, errorStream.str().c_str(), "Error", MB_OK);
		return false;
	}

	int meshCount = cal3dRenderer->getMeshCount();

	// render all meshes of the model
	for(int meshId = 0; meshId < meshCount; meshId++)
	{
		// get the number of submeshes
		int submeshCount = 1;//cal3dRenderer->getSubmeshCount(meshId);

		// render all submeshes of the mesh
		for(int submeshId = 0; submeshId < submeshCount; submeshId++)
		{
			// select mesh and submesh for further data access
			if(cal3dRenderer->selectMeshSubmesh(meshId, submeshId))
			{
				verticesSubmesh = new VertexType[cal3dRenderer->getVertexCount()];
				if(!verticesSubmesh)
				{
					MessageBox(NULL, L"No vertices in submesh.", L"Error", MB_OK);
					return false;
				}
				vertexCountSubmesh = cal3dRenderer->getVerticesNormalsAndTexCoords(&verticesSubmesh->position.x);

				// Create the index array.
				indicesSubmesh = new CalIndex[cal3dRenderer->getFaceCount()*3];
				if(!indicesSubmesh)
				{
					MessageBox(NULL, L"No faces in submesh.", L"Error", MB_OK);
					return false;
				}
				indexCountSubmesh = cal3dRenderer->getFaces(indicesSubmesh)*3;

				//Save in temp arrays the already stored data
				VertexType *verticesTemp = new VertexType[vertexCount_];
				std::memcpy(verticesTemp, vertices, sizeof(VertexType)*vertexCount_);
				delete [] vertices;

				CalIndex *indicesTemp = new CalIndex[indexCount_];
				std::memcpy(indicesTemp, indices, sizeof(CalIndex)*indexCount_);
				delete [] indices;

				//We update the total amount of vertices and indices
				vertexCount_ += vertexCountSubmesh;
				indexCount_ += indexCountSubmesh;

				//Update the new arrays for vertices and indices
				vertices = new VertexType[vertexCount_];
				indices = new CalIndex[indexCount_];

				//We copy in thew arrays the old and the new data
				memcpy(vertices, verticesTemp, sizeof(VertexType)*(vertexCount_-vertexCountSubmesh));
				for(int i = vertexCount_-vertexCountSubmesh; i < vertexCount_; i++)
				{
					vertices[i] = verticesSubmesh[i-(vertexCount_-vertexCountSubmesh)];
				}
				memcpy(indices, indicesTemp, sizeof(CalIndex)*(indexCount_-indexCountSubmesh));
				for(int i = indexCount_-indexCountSubmesh; i < indexCount_; i++)
				{
					indices[i] = indicesSubmesh[i-(indexCount_-indexCountSubmesh)]+(vertexCount_-vertexCountSubmesh);
				}

				//Delete all useless arrays to free memory
				delete [] verticesTemp;
				delete [] indicesTemp;

				delete [] verticesSubmesh;
				delete [] indicesSubmesh;
			}
		}
	}

	// end the rendering
	cal3dRenderer->endRendering();

	std::stringstream facesStream;
	facesStream << "Number faces " << indexCount_/3;
	MessageBoxA(NULL, facesStream.str().c_str(), "Polygon count", MB_OK);

	// Set up the description of the static vertex buffer.
	vertex_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	vertex_buffer_desc.ByteWidth = sizeof(VertexType) * vertexCount_;
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertex_buffer_desc.MiscFlags = 0;
	vertex_buffer_desc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertex_data.pSysMem = vertices;
	vertex_data.SysMemPitch = 0;
	vertex_data.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &vertexBuffer_);
	if(FAILED(result))
	{
		MessageBox(NULL, L"Failed creating vertex buffer.", L"Error", MB_OK);
		return false;
	}

	// Set up the description of the static index buffer.
	index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	index_buffer_desc.ByteWidth = sizeof(CalIndex) * indexCount_;
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_buffer_desc.CPUAccessFlags = 0;
	index_buffer_desc.MiscFlags = 0;
	index_buffer_desc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	index_data.pSysMem = indices;
	index_data.SysMemPitch = 0;
	index_data.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&index_buffer_desc, &index_data, &indexBuffer_);
	if(FAILED(result))
	{
		MessageBox(NULL, L"Failed creating index buffer.", L"Error", MB_OK);
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] indices;
	indices = 0;
	
	delete [] vertices;
	vertices = 0;

	return true;
}

bool ModelClass::updateBuffers(ID3D11DeviceContext* deviceContext)
{
	VertexType *vertices = 0, *verticesSubmesh = 0, *verticesPtr = 0;
	int vertexCountSubmesh;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result;

	vertexCount_ = 0;

	//Start loading information
	CalRenderer *cal3dRenderer = cal3dModel_->getRenderer();

	// begin the rendering loop
	if(!cal3dRenderer->beginRendering()) 
		return false;

	int meshCount = cal3dRenderer->getMeshCount();

	// render all meshes of the model
	for(int meshId = 0; meshId < meshCount; meshId++)
	{
		// get the number of submeshes
		int submeshCount = 1;//cal3dRenderer->getSubmeshCount(meshId);

		// render all submeshes of the mesh
		for(int submeshId = 0; submeshId < submeshCount; submeshId++)
		{
			// select mesh and submesh for further data access
			if(cal3dRenderer->selectMeshSubmesh(meshId, submeshId))
			{
				verticesSubmesh = new VertexType[cal3dRenderer->getVertexCount()];
				if(!verticesSubmesh)
				{
					return false;
				}
				vertexCountSubmesh = cal3dRenderer->getVerticesNormalsAndTexCoords(&verticesSubmesh->position.x);

				//Save in temp arrays the already stored data
				VertexType *verticesTemp = new VertexType[vertexCount_];
				std::memcpy(verticesTemp, vertices, sizeof(VertexType)*vertexCount_);
				delete [] vertices;

				//We update the total amount of vertices and indices
				vertexCount_ += vertexCountSubmesh;

				//Update the new arrays for vertices and indices
				vertices = new VertexType[vertexCount_];

				//We copy in thew arrays the old and the new data
				memcpy(vertices, verticesTemp, sizeof(VertexType)*(vertexCount_-vertexCountSubmesh));
				for(int i = vertexCount_-vertexCountSubmesh; i < vertexCount_; i++)
				{
					vertices[i] = verticesSubmesh[i-(vertexCount_-vertexCountSubmesh)];
				}

				//Delete all useless arrays to free memory
				delete [] verticesTemp;

				delete [] verticesSubmesh;
			}
		}
	}

	// end the rendering
	cal3dRenderer->endRendering();

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(vertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * vertexCount_));

	// Unlock the vertex buffer.
	deviceContext->Unmap(vertexBuffer_, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}

void ModelClass::drawBuffers(ID3D11DeviceContext* device_context)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	device_context->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	device_context->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}

void ModelClass::destroyBuffers()
{
	// Release the index buffer.
	if(indexBuffer_)
	{
		indexBuffer_->Release();
		indexBuffer_ = 0;
	}

	// Release the vertex buffer.
	if(vertexBuffer_)
	{
		vertexBuffer_->Release();
		vertexBuffer_ = 0;
	}

	return;
}

bool ModelClass::loadTexture(ID3D11Device* device, const std::string& strFilename)
{
	bool result;


	// Create the texture object.
	texture_ = new TextureClass;
	if(!texture_)
	{
		return false;
	}

	// Initialize the texture object.
	std::string root = "../Engine/data/models/" + strFilename + "/";
	result = texture_->setup(device, root + strFilename + ".dds");
	if(!result)
	{
		MessageBoxA(NULL, "Could not load texture!", "Error", MB_OK);
		return false;
	}

	return true;
}

void ModelClass::destroyTexture()
{
	// Release the texture object.
	if(texture_)
	{
		texture_->destroy();
		delete texture_;
		texture_ = 0;
	}

	return;
}

void ModelClass::destroyModel()
{
	if(cal3dModel_)
	{
		delete cal3dModel_;
		cal3dModel_ = 0;
	}

	if(cal3dCoreModel_)
	{
		delete cal3dCoreModel_;
		cal3dCoreModel_ = 0;
	}

	return;
}

bool ModelClass::parseBasicModelConfiguration(const std::string& strFilename)
{
	//We create the model Loader
	CalLoader::setLoadingMode( LOADER_ROTATE_X_AXIS );
	//We set the basic root for getting the models
	std::string root = "../Engine/data/models/" + strFilename + "/";

	//Loading the skeleton
	bool is_ok = cal3dCoreModel_->loadCoreSkeleton( root + strFilename + ".csf" );
	if(!is_ok)
	{
		MessageBoxA(NULL, "Could not load the skeleton!", "Model Error", MB_OK);
		int errorCode = CalError::getLastErrorCode();
		std::string errorString = CalError::getLastErrorDescription();
		std::string errorFile = CalError::getLastErrorFile();
		int errorLine = CalError::getLastErrorLine();
		std::stringstream errorStream;
		errorStream << "Error number " << errorCode << ": " << errorString << ". File: " << errorFile << " line " << errorLine;
		MessageBoxA(NULL, errorStream.str().c_str(), "Error", MB_OK);
		return false;
	}

	//Loading the mesh
	modelMeshID = cal3dCoreModel_->loadCoreMesh( root + strFilename + ".cmf" );
	if(modelMeshID < 0)
	{
		MessageBoxA(NULL, "Could not load the mesh!", "Model Error", MB_OK);
		int errorCode = CalError::getLastErrorCode();
		std::string errorString = CalError::getLastErrorDescription();
		std::string errorFile = CalError::getLastErrorFile();
		int errorLine = CalError::getLastErrorLine();
		std::stringstream errorStream;
		errorStream << "Error number " << errorCode << ": " << errorString << ". File: " << errorFile << " line " << errorLine;
		MessageBoxA(NULL, errorStream.str().c_str(), "Error", MB_OK);
		return false;
	}

	//Loading animations XML file
	pugi::xml_document animationsDoc;
	std::string animationsFile = root + "anims/" + strFilename + ".xml";
	if (!animationsDoc.load_file(animationsFile.c_str()))
	{
		MessageBoxA(NULL, "Could not load animations .xml file!", "Model Error", MB_OK);
		return false;
	}

	//Searching for the initial node where all "anim" nodes should be
	pugi::xml_node animationsNode;
	if(!(animationsNode = animationsDoc.child(strFilename.c_str())))
	{
		MessageBoxA(NULL, "Invalid .xml file! Could not find base node (must have model name).", "Model Error", MB_OK);
		return false;
	}

	//For each one of the nodes of the main node
	for (pugi::xml_node node = animationsNode.first_child(); node; node = node.next_sibling())
	{
		std::string node_name = node.name();
		//If the node is an anim node, we process it
		if (node_name == "anim")
		{
			//Get the name of the node
			std::string animationName;
			animationName = node.attribute("name").as_string();
			//Get the file
			std::string animationFile = root + "anims/" + animationName + ".caf";
			int animationInt = cal3dCoreModel_->loadCoreAnimation(animationFile.c_str());
			if(animationInt < 0)
			{
				std::string error = "Could not load " + animationFile;
				MessageBoxA(NULL, error.c_str(), "Model Error", MB_OK);
				int errorCode = CalError::getLastErrorCode();
				std::string errorString = CalError::getLastErrorDescription();
				std::string errorFile = CalError::getLastErrorFile();
				int errorLine = CalError::getLastErrorLine();
				std::stringstream errorStream;
				errorStream << "Error number " << errorCode << ": " << errorString << ". File: " << errorFile << " line " << errorLine;
				MessageBoxA(NULL, errorStream.str().c_str(), "Error", MB_OK);
				return false;
			}
			//Insert it into the animation map
			animations_.insert(std::pair<std::string, int>(animationName, animationInt));
			if(animationToPlay_ == "")
			{
				animationToPlay_ = animationName;
			}
		}
	}

	//Set up the model to the first loaded animation
	cal3dModel_ = new CalModel(cal3dCoreModel_);
	if(!cal3dModel_->attachMesh(modelMeshID))
	{
		MessageBox(NULL, L"Could not attach a Mesh to the model", L"Error", MB_OK);
		int errorCode = CalError::getLastErrorCode();
		std::string errorString = CalError::getLastErrorDescription();
		std::string errorFile = CalError::getLastErrorFile();
		int errorLine = CalError::getLastErrorLine();
		std::stringstream errorStream;
		errorStream << "Error number " << errorCode << ": " << errorString << ". File: " << errorFile << " line " << errorLine;
		MessageBoxA(NULL, errorStream.str().c_str(), "Error", MB_OK);
		return false;
	}

	if(!cal3dModel_->getMixer()->blendCycle(animations_.at(animationToPlay_), 1.0f, 0.0f))
	{
		MessageBox(NULL, L"Could not start initial base animation", L"Error", MB_OK);
		int errorCode = CalError::getLastErrorCode();
		std::string errorString = CalError::getLastErrorDescription();
		std::string errorFile = CalError::getLastErrorFile();
		int errorLine = CalError::getLastErrorLine();
		std::stringstream errorStream;
		errorStream << "Error number " << errorCode << ": " << errorString << ". File: " << errorFile << " line " << errorLine;
		MessageBoxA(NULL, errorStream.str().c_str(), "Error", MB_OK);
		return false;
	}

	initialAnimationToPlay_ = animationToPlay_;
	cal3dModel_->update(0.0f);

	return true;
}
