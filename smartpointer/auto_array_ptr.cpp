
template<class X> ACE_INLINE X *
ACE_Auto_Basic_Array_Ptr<X>::get (void) const
{
  return this->p_;
}

template<class X> ACE_INLINE X *
ACE_Auto_Basic_Array_Ptr<X>::release (void)
{
  X *old = this->p_;
  this->p_ = 0;
  return old;
}

template<class X> ACE_INLINE void
ACE_Auto_Basic_Array_Ptr<X>::reset (X *p)
{
  if (this->get () != p)
    delete [] this->get ();
  this->p_ = p;
}

template<class X> ACE_INLINE
ACE_Auto_Basic_Array_Ptr<X>::ACE_Auto_Basic_Array_Ptr (ACE_Auto_Basic_Array_Ptr<X> &rhs)
  : p_ (rhs.release ())
{
}

template<class X> ACE_INLINE ACE_Auto_Basic_Array_Ptr<X> &
ACE_Auto_Basic_Array_Ptr<X>::operator= (ACE_Auto_Basic_Array_Ptr<X> &rhs)
{
  if (this != &rhs)
    {
      this->reset (rhs.release ());
    }
  return *this;
}

template<class X> ACE_INLINE
ACE_Auto_Basic_Array_Ptr<X>::~ACE_Auto_Basic_Array_Ptr (void)
{
  delete [] this->get ();
}

template<class X> ACE_INLINE X &
ACE_Auto_Basic_Array_Ptr<X>::operator *() const
{
  return *this->get ();
}

template<class X> ACE_INLINE X &
ACE_Auto_Basic_Array_Ptr<X>::operator[](int i) const
{
  X *array = this->get ();
  return array[i];
}

template<class X> ACE_INLINE X *
ACE_Auto_Array_Ptr<X>::operator->() const
{
  return this->get ();
}
