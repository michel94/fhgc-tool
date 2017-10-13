package it.unimi.dsi.law.warc.io;

/*		 
 * Copyright (C) 2004-2015 Paolo Boldi, Massimo Santini and Sebastiano Vigna 
 *
 *  This library is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by the Free
 *  Software Foundation; either version 3 of the License, or (at your option)
 *  any later version.
 *
 *  This library is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 */

import it.unimi.dsi.fastutil.io.FastBufferedInputStream;
import it.unimi.dsi.law.warc.filters.Filter;
import it.unimi.dsi.law.warc.io.WarcRecord.FormatException;
import it.unimi.dsi.logging.ProgressLogger;

import java.io.IOException;
import java.util.Iterator;
import java.util.NoSuchElementException;

// RELEASE-STATUS: DIST

/** A class to iterate over WARC files getting only records that satisfy a given filter. */

public class WarcFilteredIterator implements Iterator<WarcRecord> {

	private final FastBufferedInputStream in;
	private final WarcRecord record;
	private final Filter<WarcRecord> filter;
	private final ProgressLogger pl;
	private boolean eofIsReached;
	private boolean cached;
	
	/**
	 * Builds the filtered iterator.
	 * 
	 * <p> This constructor takes a {@link WarcRecord} (or a {@link GZWarcRecord} if the stream
	 * contains compressed records) that will be reused (and thus modified) by calls to
	 * {@link #hasNext()} and {@link #next()}.
	 * 
	 * @param in the input stream.
	 * @param record the record used for reading.
	 * @param filter the filter.
	 * @param pl a (pre-initialized) {@link ProgressLogger} that will be updated during reads.
	 */
	public WarcFilteredIterator( final FastBufferedInputStream in, final WarcRecord record, final Filter<WarcRecord> filter, final ProgressLogger pl ) {
		this.in = in;
		this.record = record;
		this.filter = filter;
		this.pl = pl;
		eofIsReached = false;
		cached = false;
	}

	/**
	 * Builds the filtered iterator.
	 * 
	 * <p> This constructor takes a {@link WarcRecord} (or a {@link GZWarcRecord} if the stream
	 * contains compressed records) that will be reused (and thus modified) by calls to
	 * {@link #hasNext()} and {@link #next()}.
	 * 
	 * @param in the input stream.
	 * @param record the record used for reading.
	 * @param filter the filter.
	 */
	public WarcFilteredIterator( final FastBufferedInputStream in, final WarcRecord record, final Filter<WarcRecord> filter ) {
		this( in, record, filter, null );
	}
	
	public boolean hasNext() {
		if ( eofIsReached ) return false;
		if ( cached ) return true;
		try {
			long read;
			do {
				read = record.read( in ); 
				if ( pl != null && read != -1 ) pl.update();	
			}			
			while ( read != -1 && ! filter.apply( record ) );
			eofIsReached = read == -1;
			cached = ! eofIsReached;
		} catch ( IOException e ) {
			throw new RuntimeException( "IOException while reading next record", e );
		} catch ( FormatException e ) {
			throw new RuntimeException( "FormatException while reading next record", e );
		} 
		return ! eofIsReached;
	}

	public WarcRecord next() {
		if ( ! hasNext() ) throw new NoSuchElementException();
		cached = false;
		return record;
	}

	public void remove() {
		throw new UnsupportedOperationException();
	}

}
