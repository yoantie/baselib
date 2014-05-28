class ACE_Export ACE_Section_Key_Internal
{
public:
  /// Virtual destructor, make sure descendants are virtual!
  virtual ~ACE_Section_Key_Internal (void);

  /// Increment reference count
  virtual int add_ref (void);

  /// Decrement reference count.  Will delete this if count gets to 0
  virtual int dec_ref (void);
protected:
  ACE_Section_Key_Internal (void);
  ACE_Section_Key_Internal (const ACE_Section_Key_Internal& rhs);
  ACE_Section_Key_Internal& operator= (ACE_Section_Key_Internal& rhs);

  u_int ref_count_;
};


class ACE_Export ACE_Configuration_Section_Key
{
  friend class ACE_Configuration;
public:
  /// Default constructor.
  ACE_Configuration_Section_Key (void);

  /// Constructor that initializes to a pointer to a concrete internal key.
  /**
   * @param key The section key to reference. Calls add_ref() with @a key.
   */
  explicit ACE_Configuration_Section_Key (ACE_Section_Key_Internal *key);

  /// Copy constructor, increments the reference count on the key.
  ACE_Configuration_Section_Key (const ACE_Configuration_Section_Key &rhs);

  /// Destructor, decrements reference count on the referenced key.
  ~ACE_Configuration_Section_Key (void);

  /// Assignment operator, increments reference count for this object
  /// and decrements it on @a rhs.
  ACE_Configuration_Section_Key &
    operator= (const ACE_Configuration_Section_Key &rhs);
private:
  ACE_Section_Key_Internal *key_;
};
