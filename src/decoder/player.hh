#ifndef PLAYER_HH
#define PLAYER_HH

#include <string>
#include <vector>
#include <memory>

#include "ivf.hh"
#include "frame_info.hh"
#include "decoder.hh"
#include "serialized_frame.hh"

struct FrameRawData
{
  Chunk chunk;
  uint64_t offset;
  uint32_t length;
};

class FramePlayer
{
friend std::ostream& operator<<( std::ostream &, const FramePlayer &);
private:
  uint16_t width_, height_;

protected:
  Decoder decoder_; // FIXME ideally this would be private
  Optional<RasterHandle> decode( const Chunk & chunk );

public:
  FramePlayer( const uint16_t width, const uint16_t height );

  Optional<RasterHandle> decode( const SerializedFrame & frame );

  const VP8Raster & example_raster( void ) const;

  bool can_decode( const SerializedFrame & frame ) const;

  uint16_t width( void ) const { return width_; }
  uint16_t height( void ) const { return height_; }

  bool operator==( const FramePlayer & other ) const;
  bool operator!=( const FramePlayer & other ) const;
};

class FilePlayer : public FramePlayer
{
private:
  IVF file_;
  unsigned int frame_no_ { 0 };
  FilePlayer( IVF && file );

protected:
  FrameRawData get_next_frame();

public:
  FilePlayer( const std::string & file_name );
  RasterHandle advance();
  bool eof() const;
  unsigned int cur_frame_no() const { return frame_no_ - 1; }

  long unsigned int original_size() const;
};

using Player = FilePlayer;

#endif
