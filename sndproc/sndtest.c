#include <stdio.h>
#include <string.h>

#include <sndfile.h>

#define BUFFER_LEN 1024
#define MAX_CHANNELS 6

static void process_data (double *data, int count, int channels);

int main (int argc, char *argv[])
{
  if (argc != 2) {
    printf("%s [file]\n", argv[0]);
    return 1;
  }

  static double data[BUFFER_LEN];

  SNDFILE *infile, *outfile;
  SF_INFO sfinfo;

  int readcount;
  char *infilename = argv[1];
  const char *outfilename = "output.wav";

  memset(&sfinfo, 0, sizeof(sfinfo));

  if (!(infile = sf_open(infilename, SFM_READ, &sfinfo))) {
    puts(sf_strerror(NULL));
    return 1;
  }

  if (sfinfo.channels > MAX_CHANNELS) {
    return 1;
  }

  if (!(outfile = sf_open(outfilename, SFM_WRITE, &sfinfo))) {
    puts(sf_strerror(NULL));
    return 1;
  }

  while ((readcount = sf_read_double(infile, data, BUFFER_LEN))) {
    process_data(data, readcount, sfinfo.channels);
    sf_write_double(outfile, data, readcount);
  }

  sf_close(infile);
  sf_close(outfile);

  return 0;
}

static void process_data (double *data, int count, int channels)
{
  double channel_gain[MAX_CHANNELS] = {3.0, 0.8, 0.1, 0.4, 0.4, 0.9};
  int k, chan;

  for (chan = 0; chan < channels; chan++)
    for (k = chan; k < count; k += channels)
      data[k] *= channel_gain[chan];

  return;
}
