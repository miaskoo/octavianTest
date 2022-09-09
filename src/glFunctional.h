enum class typeVariable {UBYTE, UFLOAT}

unsigned int genVertexBuffer();
void bindVertexBuffer(unsigned int idBuff);
unsigned int genVertexArray();
void bindVertexArray(unsigned int idArray);
void copyBufferDataStatic(unsigned int idx, void* array, typeVariable typeArray, unsigned int sizeArray)
