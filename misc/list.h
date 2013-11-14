TList::TList(void)
{
	Init();
}

void TList::Init(void)
{
	top.prior = top.next = &top;
	num = 0;
}

void TList::AddObj(TListObj * obj)
{
	obj->prior = top.prior;
	obj->next = &top;
	top.prior->next = obj;
	top.prior = obj;
	num++;
}

void TList::DelObj(TListObj * obj)
{
//	if (!obj->next || !obj->prior || obj->next != &top && obj->prior != &top) {
//		Debug("DelObj(%p) (%p/%p)\n", obj, obj->next, obj->prior);
//	}
	if (obj->next) {
		obj->next->prior = obj->prior;
	}
	if (obj->prior) {
		obj->prior->next = obj->next;
	}
	obj->next = obj->prior = NULL;
	num--;
}

TListObj* TList::TopObj(void)
{
//	if (top.next != &top && top.next->next != &top && top.next->prior != &top) {
//		Debug("TopObj(%p) \n", top.next);
//	}
	return	top.next == &top ? NULL : top.next;
}

TListObj* TList::EndObj(void)
{
	return	top.next == &top ? NULL : top.prior;
}

TListObj* TList::NextObj(TListObj *obj)
{
	return	obj->next == &top ? NULL : obj->next;
}

TListObj* TList::PriorObj(TListObj *obj)
{
	return	obj->prior == &top ? NULL : obj->prior;
}

/*
	TRecycleList class
*/
TRecycleList::~TRecycleList()
{
	delete [] data;
}

TListObj *TRecycleList::GetObj(int list_type)
{
	TListObj	*obj = list[list_type].TopObj();

	if (obj)
		list[list_type].DelObj(obj);

	return	obj;
}

void TRecycleList::PutObj(int list_type, TListObj *obj)
{
	list[list_type].AddObj(obj);
}
