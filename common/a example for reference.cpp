
ACE_Section_Key_Internal::ACE_Section_Key_Internal (void)
  : ref_count_ (0)
{
}

ACE_Section_Key_Internal::~ACE_Section_Key_Internal (void)
{
}

int
ACE_Section_Key_Internal::add_ref (void)
{
  ++ref_count_;
  return 0;
}

int
ACE_Section_Key_Internal::dec_ref (void)
{
  if (!--ref_count_)
    delete this;
  return 0;
}

/*****************************************************/


ACE_Configuration_Section_Key::ACE_Configuration_Section_Key (void)
  : key_ (0)
{
}

ACE_Configuration_Section_Key::~ACE_Configuration_Section_Key (void)
{
  if (key_)
    key_->dec_ref ();
}

ACE_Configuration_Section_Key::ACE_Configuration_Section_Key (const ACE_Configuration_Section_Key& rhs)
  : key_ (rhs.key_)
{
  if (key_)
    key_->add_ref ();
}

ACE_Configuration_Section_Key&
ACE_Configuration_Section_Key::operator= (const ACE_Configuration_Section_Key& rhs)
{
  if (this != &rhs)
    {
      if (key_)
        key_->dec_ref ();

      key_ = rhs.key_;

      if (key_)
        key_->add_ref ();
    }
  return *this;
}
