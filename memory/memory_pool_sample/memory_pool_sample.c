struct memblock
{
   int              used;
   void*            data;
   struct memblock* next;
   struct memblock* createnext;
};

 

struct mempool
{
    int            size;//memblock大小
    int            unused;//空闲的memblock大小
    int            datasize;//每次分配的数据大小(就是memblock.data)
    struct memblock*    free_linkhead;//空闲memblock链表头
    struct memblock*    create_linkhead;//所有创建的memblock链表头，内存池释放的时候使用，防止内存池释放的似乎还有memblock未归还的情况
    
};
typedef void (*free_callback)(void*);//释放回调函数，释放membloc.data用，可以简单的直接用free函数

void    mempool_init(int initialSize,int datasize);//初始化mempool
void    mempool_dealloc(struct mempool* pool,free_callback callback);//释放mempool
void*    mempool_get(struct mempool* pool);//获取一个memblock
void    mempool_release(struct mempool* pool,struct memblock* block);//归还一个memblock

/*********************************
 * mempool
 * ******************************/
//malloc一个memblock
static struct memblock* mempool_allocblock( struct mempool* pool );

//------------------implement--------
void*
mempool_init( int initialSize, int datasize )
{
    struct mempool* pool = malloc( sizeof( struct mempool ) );
    pool->unused = 0;
    pool->datasize = datasize;
    pool->free_linkhead = NULL;
  
    //预先初始化initialSize个内存块
     pool->create_linkhead = NULL;
    int i;
    for ( i = 0; i < initialSize; i++ ) {
        struct memblock* block = mempool_allocblock( pool );
        mempool_release( pool, block );
    }
    return ( pool );
}

void
mempool_dealloc( struct mempool* pool, free_callback callback )
{
    struct memblock* block = NULL;
    //将所有创建的memblock释放了
    while ( pool->create_linkhead != NULL ) {
        block = pool->create_linkhead;
        pool->create_linkhead = pool->create_linkhead->createnext;
    //执行free回调。
        if ( callback ) {
            ( *callback )( block->data );
        }
        free( block );
    }
    free( pool );
    L_DEBUG( "%s:size(%d),unused(%d)", __func__, pool->size, pool->unused );
}

static struct memblock*
mempool_allocblock( struct mempool* pool )
{
    struct memblock* block = malloc( sizeof( struct memblock ) );
    block->data = malloc( sizeof( pool->datasize ) );
    block->next = NULL;
    block->used = 1;//表示已使用

    //加入所有创建的memblock的链表头
    block->createnext = pool->create_linkhead;
    pool->create_linkhead = block;

    pool->size++;
    return ( block );
}

void
mempool_release( struct mempool* pool, struct memblock* block )
{
    if ( block == NULL ) {
        L_WARN( "%s:release a NULL!", __func__ );
        return;
    }
    if ( block->used != 1 ) {
        L_WARN( "%s:used!=1", __func__ );
        return;
    }
    //将归还的内存块放到空闲链表头。
    block->used = 0;//表示空闲
    block->next = pool->free_linkhead;
    pool->free_linkhead = block;
    pool->unused++;//空闲数+1
}

void*
mempool_get( struct mempool* pool )
{
   
    struct memblock* block = NULL;
    if ( pool->free_linkhead ) {
    //从空闲链表头取出一个内存块
        block = pool->free_linkhead;
        pool->free_linkhead = pool->free_linkhead->next;
        block->next = NULL;
        block->used = 1;//表示已使用
        pool->unused--;//空闲内存块数-1
    }
    else {
    //没有空闲的内存块，创建一个
        block = mempool_allocblock( pool );
    }
    return ( block );
}

 