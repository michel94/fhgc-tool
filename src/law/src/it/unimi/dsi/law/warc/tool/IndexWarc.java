package it.unimi.dsi.law.warc.tool;

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
import it.unimi.dsi.fastutil.io.FastBufferedOutputStream;
import it.unimi.dsi.law.warc.io.GZWarcRecord;
import it.unimi.dsi.law.warc.io.WarcRecord;
import it.unimi.dsi.law.warc.io.WarcRecord.FormatException;
import it.unimi.dsi.logging.ProgressLogger;

import java.io.DataOutput;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.martiansoftware.jsap.JSAP;
import com.martiansoftware.jsap.JSAPResult;
import com.martiansoftware.jsap.Parameter;
import com.martiansoftware.jsap.SimpleJSAP;
import com.martiansoftware.jsap.Switch;
import com.martiansoftware.jsap.UnflaggedOption;

// RELEASE-STATUS: DIST

/** A tool to index a WARC file. */

public class IndexWarc {
	private final static Logger LOGGER = LoggerFactory.getLogger( IndexWarc.class );

	/**
	 * This method reads from a given input stream a sequence of WARC records and writes to a given output stream
	 * the byte offset of the read records.
	 * 
	 * @param in the input warc stream.
	 * @param isGZipped tells if the input stream contains compressed WARC records.
	 * @param out the output index stream.
	 * @throws IOException
	 * @throws FormatException
	 */
	public static void run( final FastBufferedInputStream in, final boolean isGZipped, final OutputStream out ) throws IOException, FormatException {
		final WarcRecord inRecord = isGZipped ? new GZWarcRecord() : new WarcRecord();
		final ProgressLogger pl = new ProgressLogger( LOGGER, "records" );
		final DataOutput dout = new DataOutputStream( out );
		pl.logInterval = ProgressLogger.TEN_SECONDS;
		
		pl.start("Indexing...");
		dout.writeLong( 0 );
		while ( inRecord.skip( in ) != -1 ) {
			dout.writeLong( in.position() );
			pl.update();
		}
		pl.done();
	}

	final static int IO_BUFFER_SIZE = 64 * 1024;

	public static void main(String arg[]) throws Exception {
		SimpleJSAP jsap = new SimpleJSAP( IndexWarc.class.getName(), "Index a warc file.",
				new Parameter[] { 
			new Switch( "gzip", 'z', "gzip", "Tells if the warc is compressed." ), 
			new UnflaggedOption("warcFile", JSAP.STRING_PARSER, "-", JSAP.REQUIRED,	JSAP.NOT_GREEDY, "The Warc file basename (if not present, or -, stdin/stdout will be used).")
			} );
		
		JSAPResult jsapResult = jsap.parse(arg);
		if ( jsap.messagePrinted() )	return;

		final String warcFile = jsapResult.getString("warcFile");
		final boolean isGZipped = jsapResult.getBoolean( "gzip" );

		final FastBufferedInputStream in = new FastBufferedInputStream( warcFile.equals( "-" ) ? System.in : new FileInputStream( new File( warcFile + ".warc" + ( isGZipped ? ".gz" : "" ) ) ), IO_BUFFER_SIZE );
		final FastBufferedOutputStream out = new FastBufferedOutputStream( warcFile.equals("-") ? System.out : new FileOutputStream( new File( warcFile + ".warc" + ( isGZipped ? ".gz" : "" ) + ".idx" ) ), IO_BUFFER_SIZE );

		run( in, isGZipped, out );

		in.close();
		out.close();
	}
}
