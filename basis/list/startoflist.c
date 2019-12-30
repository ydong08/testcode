/*串长a + n，其中n为循环，当a + b步的慢指针与快指针相遇时，快指针已经走过了k圈。
即a + b + k * n = 2 * (a+b)，求a，得到a = k * n - b。
也就是X走a步，等于Z位置上的指针再走k圈，相遇于Y点。*/
 
ListNode *detectCycle(ListNode *h) {
        ListNode *p = h, *q = h;
        while (q && q->next) {
            p = p->next;
            q = q->next->next;
            if (p == q) {
                while (h != q) {
                    q = q->next;
                    h = h->next;
                }
                return h;
            }
        }
          
        return NULL;
    }