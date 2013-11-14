struct TListObj {
	TListObj *prior, *next;
};

class TList {
protected:
	TListObj	top;
	int			num;

public:
	TList(void);
	void		Init(void);
	void		AddObj(TListObj *obj);
	void		DelObj(TListObj *obj);
	TListObj	*TopObj(void);
	TListObj	*EndObj(void);
	TListObj	*NextObj(TListObj *obj);
	TListObj	*PriorObj(TListObj *obj);
	BOOL		IsEmpty() { return	top.next == &top; }
	void		MoveList(TList *from_list);
	int			Num() { return num; }
};

class TIniKey : public TListObj {
protected:
	char	*key;	// null means comment line
	char	*val;

public:
	TIniKey(const char *_key=NULL, const char *_val=NULL) {
		key = val = NULL; if (_key || _val) Set(_key, _val);
	}
	~TIniKey() { free(key); free(val); }

	void Set(const char *_key=NULL, const char *_val=NULL) {
		if (_key) { free(key); key=strdup(_key); }
		if (_val) { free(val); val=strdup(_val); }
	}
	const char *Key() { return key; }
	const char *Val() { return val; }
};

#define FREE_LIST	0
#define USED_LIST	1
#define	RLIST_MAX	2
class TRecycleList {
protected:
	char	*data;
	TList	list[RLIST_MAX];

public:
	TRecycleList(int init_cnt, int size);
	~TRecycleList();
	TListObj *GetObj(int list_type);
	void PutObj(int list_type, TListObj *obj);
};

#define IsWinNT()		(LOBYTE(LOWORD(TWinVersion)) >= 4 && TWinVersion < 0x80000000)
#define IsWin2K()		(LOBYTE(LOWORD(TWinVersion)) >= 5 && TWinVersion < 0x80000000)
#define IsWinXP()		((LOBYTE(LOWORD(TWinVersion)) >= 6 || LOBYTE(LOWORD(TWinVersion)) == 5 \
							&& HIBYTE(LOWORD(TWinVersion)) >= 1) && TWinVersion < 0x80000000)
#define IsWinVista()	(LOBYTE(LOWORD(TWinVersion)) >= 6 && TWinVersion < 0x80000000)
#define IsWin7()		((LOBYTE(LOWORD(TWinVersion)) >= 7 || LOBYTE(LOWORD(TWinVersion)) == 6 \
							&& HIBYTE(LOWORD(TWinVersion)) >= 1) && TWinVersion < 0x80000000)
