//----------------------------------------------------------------------
// tLogDomain destructor
//----------------------------------------------------------------------
tLogDomain::~tLogDomain()
{
  if (this->file_stream.is_open())
  {
    this->file_stream.close();
  }
}

//----------------------------------------------------------------------
// tLogDomain SetupOutputStream
//----------------------------------------------------------------------
void tLogDomain::SetupOutputStream(int mask) const
{
  this->stream_buffer.Clear();
  if (mask & (1 << eLS_STDOUT))
  {
    this->stream_buffer.AddStream(std::cout);
  }
  if (mask & (1 << eLS_STDERR))
  {
    this->stream_buffer.AddStream(std::cerr);
  }
  if (mask & (1 << eLS_FILE))
  {
    this->stream_buffer.AddStream(this->OpenFileOutputStream() ? this->file_stream : std::cerr);
  }
  if (mask & (1 << eLS_COMBINED_FILE))
  {
    const tLogDomain *domain = this;
    for (; domain->parent && domain->parent->configuration->configure_sub_tree; domain = domain->parent);
    this->stream_buffer.AddStream(domain->OpenFileOutputStream() ? domain->file_stream : std::cerr);
  }
}

//----------------------------------------------------------------------
// tLogDomain OpenFileOutputStream
//----------------------------------------------------------------------
const bool tLogDomain::OpenFileOutputStream() const
{
  if (this->file_stream.is_open())
  {
    return true;
  }
  const std::string &file_name_prefix(tLogDomainRegistry::GetInstance()->GetOutputFileNamePrefix());
  if (file_name_prefix.length() == 0)
  {
    std::cerr << "RRLib Logging >> Prefix for file names not set. Can not use eMS_FILE." << std::endl
              << "                 Consider calling tLogDomainRegistry::GetInstance()->SetOutputFileNamePrefix(basename(argv[0])) for example." << std::endl;
    return false;
  }
  std::string file_name(file_name_prefix + this->GetName() + ".log");
  this->file_stream.open(file_name.c_str(), std::ios::out | std::ios::trunc);
  if (!this->file_stream.is_open())
  {
    std::cerr << "RRLib Logging >> Could not open file `" << file_name << "'!" << std::endl;
    return false;
  }
  return true;
}

