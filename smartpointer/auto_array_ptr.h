#define ACE_INLINE

template<typename X>
class ACE_Auto_Basic_Array_Ptr
{
public:
  typedef X element_type;

  // = Initialization and termination methods.
  explicit ACE_Auto_Basic_Array_Ptr (X * p = 0) : p_ (p) {}

  ACE_Auto_Basic_Array_Ptr (ACE_Auto_Basic_Array_Ptr<X> & ap);
  ACE_Auto_Basic_Array_Ptr<X> &operator= (ACE_Auto_Basic_Array_Ptr<X> & rhs);
  ~ACE_Auto_Basic_Array_Ptr (void);

  // = Accessor methods.
  // must be "&", for operation like: p[0] = 'a'
  X & operator* () const;
  X & operator[] (int i) const;
  X * get (void) const;
  X * release (void);
  void reset (X * p = 0);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.


protected:
  X * p_;
};

template<typename X>
class ACE_Auto_Array_Ptr : public ACE_Auto_Basic_Array_Ptr<X>
{
public:
  typedef X element_type;

  // = Initialization and termination methods.
  explicit ACE_Auto_Array_Ptr (X *p = 0)
    : ACE_Auto_Basic_Array_Ptr<X> (p) {}

  X *operator-> () const;
};
