/*! The dtor of tLogDomain
 */
~tLogDomain();

mutable std::ofstream file_stream;

/*! Open the file stream for file output
 *
 * This method creates a new file which name is build using a prefix
 * and the full qualified domain name.
 * If the file already exists, it will be truncated.
 *
 * \returns Whether the file stream could be opened or not
 */
const bool OpenFileOutputStream() const;

/*! Setup the output stream to be used in this domain
 *
 * A domain can stream its input to stdout, stderr, an own file and/or its parent's file.
 *
 *\param mask   The bitmask that selects the output streams to use
 */
void SetupOutputStream(int mask) const;
